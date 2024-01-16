// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioDeviceManager.h"
#include "MovieSceneCapture/Public/MovieSceneCapture.h"

/**
 * MovieSceneCaptureHelper
 */
class MovieSceneCaptureHelper
{
private:
	// 禁止外部构造
	MovieSceneCaptureHelper();

	// 禁止外部析构
	~MovieSceneCaptureHelper() {}

	// 禁止外部复制构造
	MovieSceneCaptureHelper(const MovieSceneCaptureHelper& InMovieSceneCaptureHelper) = delete;

	// 禁止外部赋值操作
	MovieSceneCaptureHelper& operator=(const MovieSceneCaptureHelper& InMovieSceneCaptureHelper) = delete;

public:
	// 获取单实例对象
	static MovieSceneCaptureHelper& GetInstance();

private:
	UMovieSceneCapture* MovieSceneCapture;

	UWorld* CapturingFromWorld;

	bool bScreenMessagesWereEnabled;
	float TransientMasterVolume;
	int32 BackedUpStreamingPoolSize;
	int32 BackedUpUseFixedPoolSize;
	int32 BackedUpTextureStreaming;
	TArray<uint8> BackedUpPlaySettings;

	bool CachedPathTracingMode = false;
	struct FEngineShowFlags* CachedEngineShowFlags = nullptr;
	TSubclassOf<AGameModeBase> CachedGameMode;

	/** Overrides the Level Editor Play settings to specifically disable some things (such as audio playback) */
	void OverridePlaySettings(class ULevelEditorPlaySettings* PlayInEditorSettings);
	/** Called when the PIE Viewport is created. */
	void OnPIEViewportStarted();
	/** Shuts down the capture setup , called when PIE is closed by the user or the sequence finishes playing. */
	void Shutdown();
	/** Called when the user closes the PIE instance window. */
	void OnEndPIE(bool bIsSimulating);
	/** Called when the Sequence finishes playing to the end. */
	void OnLevelSequenceFinished();

private:
	void InitMovieSceneCapture();

public:
	void Start();

	void Close();
};
