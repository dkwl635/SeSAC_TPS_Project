// Fill out your copyright notice in the Description page of Project Settings.
#include "TPSGameMode.h"
#include "TPSProject.h"

ATPSGameMode::ATPSGameMode ( )
{
	//PRINT_LOG(fmt, ...)
	PRINT_LOG ( TEXT ( "My_LOG_Test1 : %s" ) , TEXT ( "TPS project" ) );
	PRINT_CALLINFO ( );
	
	PRINT_LOG ( TEXT ( "My_LOG_Test2 : %s" ) , *GetName ( ) );

}
