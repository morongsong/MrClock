--
-- The Global of Solution Setting
--


the_vs_name  = ""

if not the_top_dir then
    the_top_dir = path.translate(path.getabsolute("."), "\\")
end

if _ACTION == "vs2015" then
    the_vs_name = ""
elseif _ACTION == "vs2013" then
    the_vs_name = "vs12"
elseif _ACTION == "vs2012" then
    the_vs_name = "vs11"
end   

the_vc_ver  = ""
if _ACTION == "vs2012" then
    the_vc_ver = "vc11"
elseif _ACTION == "vs2013" then
    the_vc_ver = "vc12"
elseif _ACTION == "vs2015" then
    the_vc_ver = "vc14"
end   
    
function mr_build_copyfile(src, dest)
	src      = path.translate(path.getabsolute(dest), "/")
	dest_dir = path.translate(path.getabsolute(dest), "/")
    os.mkdir(dest_dir)
    ok, err = os.execute("copy /y \""..src.."\" \""..dest.."\" 1>nul 2>nul")
end

function mr_copyfile_tobinx86(src)
	dest = the_top_dir..'/Bin/Debug'
	mr_build_copyfile(src,dest)
	dest = the_top_dir..'/Bin/Release'
	mr_build_copyfile(src,dest)
end

function mr_copyfile_tobinx64(src)
	dest = the_top_dir..'/Bin/Debugx64'
	mr_build_copyfile(src,dest)
	dest = the_top_dir..'/Bin/Releasex64'
	mr_build_copyfile(src,dest)
end

function mr_copyfile_tobin(src)
	mr_copyfile_tobinx86(src)
	mr_copyfile_tobinx64(src)
end

workspace (the_sln_name)
    configurations {'Debug', 'Release'}
    platforms {"Win32","x64"}
    location ('_Build'..the_vs_name)
	characterset ('Unicode')

	filter {"language:C++"}
		links{'DelayImp.lib'}
		defines {'_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS'}

	filter "platforms:Win32"
		architecture 'x32'
	filter "platforms:x64"
		architecture 'x64'
        
	filter {"platforms:Win32"}
		targetdir ("Bin"..the_vs_name.."/%{cfg.buildcfg}")
		implibdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}")
		debugdir  ("Bin"..the_vs_name.."/%{cfg.buildcfg}")
	filter {"platforms:x64"}
		targetdir ("Bin"..the_vs_name.."/%{cfg.buildcfg}x64")
		implibdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}x64")
		debugdir  ("Bin"..the_vs_name.."/%{cfg.buildcfg}x64")

	filter {"configurations:Debug"}
		defines { "DEBUG","_DEBUG"}
		symbols "On"
	filter {"configurations:Release"}	
		defines { "NDEBUG" }
		optimize "On"
		symbols "On"
        
    filter {'kind:StaticLib',"platforms:Win32"}
        targetdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}")
    filter {'kind:StaticLib',"platforms:x64"}
        targetdir ("Lib"..the_vs_name.."/%{cfg.buildcfg}x64")

    filter {"platforms:Win32"}
        libdirs {"Lib"..the_vs_name.."/%{cfg.buildcfg}"}
    filter {"platforms:x64"}
        libdirs {"Lib"..the_vs_name.."/%{cfg.buildcfg}x64"}

--project '_Pmake'
--    kind 'Utility'
--    files {'**.lua'}
--    vpaths {["*"] = {'**.lua'}}
--    postbuildcommands {
--		"cd /d ..\\",
--		"Premake5 ".._ACTION
--	}
--    cleancommands {"cd /d ..\\"}
