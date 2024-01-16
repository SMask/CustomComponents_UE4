// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/MovieSceneCaptureHelper.h"
#include "Framework/Application/SlateApplication.h"
#include "Serialization/ObjectWriter.h"
#include "Serialization/ObjectReader.h"
#include "MovieSceneCapture/Public/Protocols/AudioCaptureProtocol.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/WorldSettings.h"

#include "Engine/GameEngine.h"
#include "MovieSceneCapture/Public/MovieSceneCaptureModule.h"
#include "MovieScene/Public/MovieSceneTimeController.h"

MovieSceneCaptureHelper::MovieSceneCaptureHelper()
{
}

MovieSceneCaptureHelper& MovieSceneCaptureHelper::GetInstance()
{
	static MovieSceneCaptureHelper SignalInstance;
	return SignalInstance;
}

void MovieSceneCaptureHelper::OverridePlaySettings(ULevelEditorPlaySettings* PlayInEditorSettings)
{
	const FMovieSceneCaptureSettings& Settings = MovieSceneCapture->GetSettings();

	PlayInEditorSettings->NewWindowWidth = Settings.Resolution.ResX;
	PlayInEditorSettings->NewWindowHeight = Settings.Resolution.ResY;
	PlayInEditorSettings->CenterNewWindow = false;
	PlayInEditorSettings->NewWindowPosition = FIntPoint::NoneValue; // It will center PIE to the middle of the screen the first time it is run (until the user drag the window somewhere else)
	PlayInEditorSettings->LastExecutedPlayModeType = EPlayModeType::PlayMode_InEditorFloating;

	// Reset everything else
	PlayInEditorSettings->GameGetsMouseControl = false;
	PlayInEditorSettings->ShowMouseControlLabel = false;
	PlayInEditorSettings->ViewportGetsHMDControl = false;
	PlayInEditorSettings->ShouldMinimizeEditorOnVRPIE = true;
	PlayInEditorSettings->EnableGameSound = MovieSceneCapture->AudioCaptureProtocolType != UNullAudioCaptureProtocol::StaticClass();
	PlayInEditorSettings->bOnlyLoadVisibleLevelsInPIE = false;
	PlayInEditorSettings->bPreferToStreamLevelsInPIE = false;
	PlayInEditorSettings->PIEAlwaysOnTop = false;
	PlayInEditorSettings->DisableStandaloneSound = false;
	PlayInEditorSettings->AdditionalLaunchParameters = TEXT("");
	PlayInEditorSettings->BuildGameBeforeLaunch = EPlayOnBuildMode::PlayOnBuild_Never;
	PlayInEditorSettings->LaunchConfiguration = EPlayOnLaunchConfiguration::LaunchConfig_Default;
	PlayInEditorSettings->PackFilesForLaunch = EPlayOnPakFileMode::NoPak;
	PlayInEditorSettings->SetPlayNetMode(EPlayNetMode::PIE_Standalone);
	PlayInEditorSettings->SetRunUnderOneProcess(true);
	PlayInEditorSettings->bLaunchSeparateServer = false;
	PlayInEditorSettings->SetPlayNumberOfClients(1);
}

void MovieSceneCaptureHelper::OnPIEViewportStarted()
{
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE)
		{
			FSlatePlayInEditorInfo* SlatePlayInEditorSession = GEditor->SlatePlayInEditorMap.Find(Context.ContextHandle);
			if (SlatePlayInEditorSession)
			{
				CapturingFromWorld = Context.World();

				TSharedPtr<SWindow> Window = SlatePlayInEditorSession->SlatePlayInEditorWindow.Pin();

				const FMovieSceneCaptureSettings& Settings = MovieSceneCapture->GetSettings();

				SlatePlayInEditorSession->SlatePlayInEditorWindowViewport->SetViewportSize(Settings.Resolution.ResX, Settings.Resolution.ResY);

				FVector2D PreviewWindowSize(Settings.Resolution.ResX, Settings.Resolution.ResY);

				// Keep scaling down the window size while we're bigger than half the desktop width/height
				{
					FDisplayMetrics DisplayMetrics;
					FSlateApplication::Get().GetCachedDisplayMetrics(DisplayMetrics);

					while (PreviewWindowSize.X >= DisplayMetrics.PrimaryDisplayWidth * .5f || PreviewWindowSize.Y >= DisplayMetrics.PrimaryDisplayHeight * .5f)
					{
						PreviewWindowSize *= .5f;
					}
				}

				// Resize and move the window into the desktop a bit
				FVector2D PreviewWindowPosition(50, 50);
				Window->ReshapeWindow(PreviewWindowPosition, PreviewWindowSize);

				if (MovieSceneCapture->Settings.GameModeOverride != nullptr)
				{
					CachedGameMode = CapturingFromWorld->GetWorldSettings()->DefaultGameMode;
					CapturingFromWorld->GetWorldSettings()->DefaultGameMode = MovieSceneCapture->Settings.GameModeOverride;
				}

				CachedEngineShowFlags = SlatePlayInEditorSession->SlatePlayInEditorWindowViewport->GetClient()->GetEngineShowFlags();
				if (CachedEngineShowFlags && Settings.bUsePathTracer)
				{
					CachedPathTracingMode = CachedEngineShowFlags->PathTracing;
					CachedEngineShowFlags->SetPathTracing(true);
				}

				MovieSceneCapture->Initialize(SlatePlayInEditorSession->SlatePlayInEditorWindowViewport, Context.PIEInstance);
				MovieSceneCapture->StartCapturing();
			}
			return;
		}
	}
}

void MovieSceneCaptureHelper::Shutdown()
{
	FEditorDelegates::EndPIE.RemoveAll(this);
	UGameViewportClient::OnViewportCreated().RemoveAll(this);
	MovieSceneCapture->OnCaptureFinished().RemoveAll(this);

	GAreScreenMessagesEnabled = bScreenMessagesWereEnabled;

	if (!MovieSceneCapture->Settings.bEnableTextureStreaming)
	{
		IConsoleVariable* CVarStreamingPoolSize = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.PoolSize"));
		if (CVarStreamingPoolSize)
		{
			CVarStreamingPoolSize->Set(BackedUpStreamingPoolSize, ECVF_SetByConsole);
		}

		IConsoleVariable* CVarUseFixedPoolSize = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.UseFixedPoolSize"));
		if (CVarUseFixedPoolSize)
		{
			CVarUseFixedPoolSize->Set(BackedUpUseFixedPoolSize, ECVF_SetByConsole);
		}

		IConsoleVariable* CVarTextureStreaming = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TextureStreaming"));
		if (CVarTextureStreaming)
		{
			CVarTextureStreaming->Set(BackedUpTextureStreaming, ECVF_SetByConsole);
		}
	}

	if (MovieSceneCapture->Settings.GameModeOverride != nullptr && CapturingFromWorld != nullptr)
	{
		CapturingFromWorld->GetWorldSettings()->DefaultGameMode = CachedGameMode;
	}

	if (CachedEngineShowFlags)
	{
		CachedEngineShowFlags->SetPathTracing(CachedPathTracingMode);
	}

	FObjectReader(GetMutableDefault<ULevelEditorPlaySettings>(), BackedUpPlaySettings);

	FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice();
	if (AudioDevice)
	{
		AudioDevice->SetTransientMasterVolume(TransientMasterVolume);
	}

	MovieSceneCapture->Close();
	MovieSceneCapture->RemoveFromRoot();
}

void MovieSceneCaptureHelper::OnEndPIE(bool bIsSimulating)
{
	Shutdown();
}

void MovieSceneCaptureHelper::OnLevelSequenceFinished()
{
	Shutdown();

	GEditor->RequestEndPlayMap();
}

void MovieSceneCaptureHelper::InitMovieSceneCapture()
{
	MovieSceneCapture = NewObject<UMovieSceneCapture>(GetTransientPackage());
	MovieSceneCapture->LoadFromConfig();

	// 输出路径及文件名
	MovieSceneCapture->Settings.OutputDirectory.Path = TEXT("D:/");
	MovieSceneCapture->Settings.OutputFormat = TEXT("CustomName");
	// 分辨率
	MovieSceneCapture->Settings.Resolution.ResX = 1080;
	MovieSceneCapture->Settings.Resolution.ResY = 1080;
	// 是否覆盖同名文件
	MovieSceneCapture->Settings.bOverwriteExisting = false;
	// 不启用纹理流
	MovieSceneCapture->Settings.bEnableTextureStreaming = false;
}

void MovieSceneCaptureHelper::Start()
{
	InitMovieSceneCapture();

	ULevelEditorPlaySettings* PlayInEditorSettings = GetMutableDefault<ULevelEditorPlaySettings>();

	bScreenMessagesWereEnabled = GAreScreenMessagesEnabled;
	GAreScreenMessagesEnabled = false;

	if (!MovieSceneCapture->Settings.bEnableTextureStreaming)
	{
		const int32 UndefinedTexturePoolSize = -1;
		IConsoleVariable* CVarStreamingPoolSize = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.PoolSize"));
		if (CVarStreamingPoolSize)
		{
			BackedUpStreamingPoolSize = CVarStreamingPoolSize->GetInt();
			CVarStreamingPoolSize->Set(UndefinedTexturePoolSize, ECVF_SetByConsole);
		}

		IConsoleVariable* CVarUseFixedPoolSize = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Streaming.UseFixedPoolSize"));
		if (CVarUseFixedPoolSize)
		{
			BackedUpUseFixedPoolSize = CVarUseFixedPoolSize->GetInt();
			CVarUseFixedPoolSize->Set(0, ECVF_SetByConsole);
		}

		IConsoleVariable* CVarTextureStreaming = IConsoleManager::Get().FindConsoleVariable(TEXT("r.TextureStreaming"));
		if (CVarTextureStreaming)
		{
			BackedUpTextureStreaming = CVarTextureStreaming->GetInt();
			CVarTextureStreaming->Set(0, ECVF_SetByConsole);
		}
	}

	FObjectWriter(PlayInEditorSettings, BackedUpPlaySettings);
	OverridePlaySettings(PlayInEditorSettings);

	MovieSceneCapture->AddToRoot();
	MovieSceneCapture->OnCaptureFinished().AddRaw(this, &MovieSceneCaptureHelper::OnLevelSequenceFinished);

	UGameViewportClient::OnViewportCreated().AddRaw(this, &MovieSceneCaptureHelper::OnPIEViewportStarted);
	FEditorDelegates::EndPIE.AddRaw(this, &MovieSceneCaptureHelper::OnEndPIE);

	FAudioDeviceHandle AudioDevice = GEngine->GetMainAudioDevice();
	if (AudioDevice)
	{
		TransientMasterVolume = AudioDevice->GetTransientMasterVolume();
		AudioDevice->SetTransientMasterVolume(0.0f);
	}

	TSharedRef<SWindow> CustomWindow = SNew(SWindow)
		.Title(NSLOCTEXT("MovieSceneCapture", "MovieRenderPreviewTitle", "Movie Render - Preview"))
		.AutoCenter(EAutoCenter::PrimaryWorkArea)
		.UseOSWindowBorder(true)
		.FocusWhenFirstShown(false)
		.ActivationPolicy(EWindowActivationPolicy::Never)
		.HasCloseButton(true)
		.SupportsMaximize(false)
		.SupportsMinimize(true)
		.MaxWidth(MovieSceneCapture->GetSettings().Resolution.ResX)
		.MaxHeight(MovieSceneCapture->GetSettings().Resolution.ResY)
		.SizingRule(ESizingRule::FixedSize);

	FSlateApplication::Get().AddWindow(CustomWindow);

	FRequestPlaySessionParams Params;
	Params.EditorPlaySettings = PlayInEditorSettings;
	Params.CustomPIEWindow = CustomWindow;

	GEditor->RequestPlaySession(Params);
}

void MovieSceneCaptureHelper::Close()
{
	OnLevelSequenceFinished();
}
