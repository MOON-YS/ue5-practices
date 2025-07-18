// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PrototypeL, Log, All);

#define PLLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PLLOG_S(Verbosity) UE_LOG(PrototypeL,Verbosity,TEXT("%s"),*PLLOG_CALLINFO)
#define PLLOG(Verbosity,Format, ...) UE_LOG(PrototypeL, Verbosity, TEXT("%s %s"), *PLLOG_CALLINFO, *FString::Printf(Format,##__VA_ARGS__))
#define PLCHECK(Expr, ...) {if(!(Expr)){PLLOG(Error, TEXT("ASSERTION : %s"),TEXT("'"#Expr"'")); return __VA_ARGS__;}}
#define LOG_VECTOR(vec) PLLOG(PrototypeL, Warning, TEXT("%f, %f, %f"), vec.X,vec.Y,vec.Z);

