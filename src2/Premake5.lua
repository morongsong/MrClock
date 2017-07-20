

local TheTargetName='MrClockPt'

project (TheTargetName)
    targetname (TheTargetName)
    kind ('WindowedApp')
    linkoptions {'/ENTRY:wWinMainCRTStartup'}

    includedirs {'./'}
    files {'**'}
    removefiles {'*.aps'}

	flags { "StaticRuntime" }

    --defines {'_AFXDLL'}
    
