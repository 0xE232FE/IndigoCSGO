#pragma once

#include "SDK.h"

namespace SDK
{
	namespace TABLE
	{
		namespace IGameEvent
		{
			enum
			{
				GetName = 1 ,
				GetInt = 6 ,
				GetFloat = 8 ,
				GetString = 9 ,
				SetString = 16
			};
		}

		namespace IGameEventManager2
		{
			enum
			{
				FireEventClientSide = 9
			};
		}
	}

	class IGameEvent
	{
	public:
		const char* GetName()
		{
			VirtualFn( const char* )( PVOID );
			return GetMethod< OriginalFn >( this , TABLE::IGameEvent::GetName )( this );
		}

		int GetInt( const char* szKeyName , int nDefault = 0 )
		{
			VirtualFn( int )( PVOID , const char* , int );
			return GetMethod< OriginalFn >( this , TABLE::IGameEvent::GetInt )( this , szKeyName , nDefault );
		}

		float GetFloat( const char *szkeyName = NULL , float defaultValue = 0.0f )
		{
			VirtualFn( float )( PVOID , const char* , float );
			return GetMethod< OriginalFn >( this , TABLE::IGameEvent::GetFloat )( this , szkeyName , defaultValue );
		}

		const char* GetString( const char* szKeyName )
		{
			VirtualFn( const char* )( PVOID , const char* , int );
			return GetMethod< OriginalFn >( this , TABLE::IGameEvent::GetString )( this , szKeyName , 0 );
		}

		void SetString( const char* szKeyName , const char* szValue )
		{
			VirtualFn( void )( PVOID , const char* , const char* );
			GetMethod< OriginalFn >( this , TABLE::IGameEvent::SetString )( this , szKeyName , szValue );
		}
	};

	class IGameEventManager2
	{
	public:
		bool FireEventClientSide( IGameEvent* pEvent )
		{
			VirtualFn( bool )( PVOID , IGameEvent* );
			return GetMethod< OriginalFn >( this , TABLE::IGameEventManager2::FireEventClientSide )( this , pEvent );
		}
	};
}