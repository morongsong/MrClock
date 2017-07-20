

local TheTargetName='MrClock'

project (TheTargetName)
    targetname (TheTargetName)
    kind ('WindowedApp')
    linkoptions {'/ENTRY:wWinMainCRTStartup'}

    includedirs {'./'}
    files {'**'}
    removefiles {'*.aps'}

	flags { "StaticRuntime" }

    --defines {'_AFXDLL'}
    
    pchheader ('stdafx.h')
    pchsource ('stdafx.cpp')