-- premake5.lua
workspace "CrabGui"
    configurations { "Debug", "Release" }

    if (_ACTION == "clean") then
        --os.rmdir("./_Depend")
        os.rmdir("./Core/lib")
        os.rmdir("./D3D9Renderer/lib")
        os.rmdir("./FreeParser/lib")
        os.rmdir("./StbParser/lib")
        os.rmdir("./obj")
        os.rmdir("./ipch")

        os.remove("CrabGui.sdf")
        os.remove("CrabGui.sln")
        os.remove("CrabGui.suo")
        os.remove("CrabGuiSample_FirstWindow.*")
        os.remove("CrabGui_Core.*")
        os.remove("CrabGui_D3D9Renderer.*")
        os.remove("CrabGui_FreeParser.*")
        os.remove("CrabGui_StbParser.*")

        os.remove("Output/CrabGui_Core*.*")
        os.remove("Output/CrabGui_D3D9Renderer*.*")
        os.remove("Output/CrabGui_FreeParser*.*")
        os.remove("Output/CrabGui_StbParser*.*")
        os.remove("Output/CrabGuiSample_FirstWindow*.*")
    end

-------------------------------------------------------------------------------
project "CrabGui_Core"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./Core/lib"

    files {
        "./Core/include/**.h",
        "./Core/src/**.cpp"
    }

    includedirs {
        "./Core/include"
    }

    filter "configurations:Debug"
        targetname "CrabGui_Core_d"
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        symbols "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_Core_d.dll ./Output",
                            "{COPY} %{cfg.targetdir}/CrabGui_Core_d.pdb ./Output" }

    filter "configurations:Release"
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        -- optimize "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_Core.dll ./Output" }


-------------------------------------------------------------------------------
project "CrabGui_D3D9Renderer"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./D3D9Renderer/lib"

    files { 
        "./D3D9Renderer/include/**.h",
        "./D3D9Renderer/src/**.cpp"
    }

    includedirs {
        "./Core/include",
        "./D3D9Renderer/include",
        "./_Depend/dx9sdk/include"
    }

    libdirs {
        "./Core/lib",
        "./_Depend/dx9sdk/lib/x86"
    }

    filter "configurations:Debug"
        targetname "CrabGui_D3D9Renderer_d"
        links { "CrabGui_Core_d", "d3d9", "d3dx9" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        symbols "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_D3D9Renderer_d.dll ./Output",
                            "{COPY} %{cfg.targetdir}/CrabGui_D3D9Renderer_d.pdb ./Output" }

    filter "configurations:Release"
        links { "CrabGui_Core", "d3d9", "d3dx9" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        -- optimize "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_D3D9Renderer.dll ./Output" }



-------------------------------------------------------------------------------
project "CrabGui_FreeParser"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./FreeParser/lib"

    files { 
        "./FreeParser/include/**.h",
        "./FreeParser/src/**.cpp"
    }

    includedirs {
        "./Core/include",
        "./FreeParser/include",
        "./_Depend/freeimage",
        "./_Depend/freetype",
        --"./FreeParser/depend/freetype"
    }

    libdirs {
        "./Core/lib",
        "./_Depend/freeimage",
        "./_Depend/freetype",
    }

    filter "configurations:Debug"
        targetname "CrabGui_FreeParser_d"
        links { "CrabGui_Core_d", "FreeImage", "freetype246MT" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        symbols "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_FreeParser_d.dll ./Output",
                            "{COPY} %{cfg.targetdir}/CrabGui_FreeParser_d.pdb ./Output" }

    filter "configurations:Release"
        links { "CrabGui_Core", "FreeImage", "freetype246MT" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        -- optimize "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_FreeParser.dll ./Output" }


-------------------------------------------------------------------------------
project "CrabGui_StbParser"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./StbParser/lib"

    files { 
        "./StbParser/stb_include/**.h",
        "./StbParser/include/**.h",
        "./StbParser/src/**.cpp"
    }

    includedirs {
        "./Core/include",
        "./StbParser/stb_include",
        "./StbParser/include",
    }

    libdirs {
        "./Core/lib",
    }

    filter "configurations:Debug"
        targetname "CrabGui_StbParser_d"
        links { "CrabGui_Core_d" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        symbols "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_StbParser_d.dll ./Output",
                            "{COPY} %{cfg.targetdir}/CrabGui_StbParser_d.pdb ./Output" }

    filter "configurations:Release"
        links { "CrabGui_Core" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        -- optimize "On"
        postbuildcommands { "{COPY} %{cfg.targetdir}/CrabGui_StbParser.dll ./Output" }



-------------------------------------------------------------------------------
project "CrabGuiSample_FirstWindow"
    kind "WindowedApp"
    language "C++"
    characterset "ASCII"
    targetdir "./Output"

    files { 
        "./Sample/src/**.*"
    }

    includedirs {
        "./Core/include",
        "./D3D9Renderer/include",
        "./FreeParser/include",
        "./StbParser/include",
        "./_Depend/dx9sdk/include",
    }

    libdirs {
        "./Core/lib",
        "./D3D9Renderer/lib",
        "./FreeParser/lib",
        "./StbParser/lib",
        "./_Depend/dx9sdk/lib/x86"
    }

    filter "configurations:Debug"
        targetname "CrabGuiSample_FirstWindow_d"
        links { "CrabGui_Core_d", "CrabGui_D3D9Renderer_d", "CrabGui_FreeParser_d", "CrabGui_StbParser_d", "d3d9", "d3dx9", "Winmm" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
        symbols "On"

    filter "configurations:Release"
        links { "CrabGui_Core", "CrabGui_D3D9Renderer", "CrabGui_FreeParser", "CrabGui_StbParser", "d3d9", "d3dx9", "Winmm" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
        -- optimize "On"

