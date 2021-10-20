-- premake5.lua
workspace "CrabGui"
    configurations { "Debug", "Release" }


-------------------------------------------------------------------------------
project "CrabGui_Core"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./Output"

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

    filter "configurations:Release"
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        optimize "On"


-------------------------------------------------------------------------------
project "CrabGui_D3D9Renderer"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./Output"

    files { 
        "./D3D9Renderer/include/**.h",
        "./D3D9Renderer/src/**.cpp"
    }

    includedirs {
        "./Core/include",
        "./D3D9Renderer/include",
        "./D3D9Renderer/depend/DirectX9/include"
    }

    libdirs {
        "./Output",
        "./D3D9Renderer/depend/DirectX9/lib/x86"
    }

    filter "configurations:Debug"
        targetname "CrabGui_D3D9Renderer_d"
        links { "CrabGui_Core_d", "d3d9", "d3dx9d" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        symbols "On"

    filter "configurations:Release"
        links { "CrabGui_Core", "d3d9", "d3dx9" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        optimize "On"


-------------------------------------------------------------------------------
project "CrabGui_FreeParser"
    kind "SharedLib"
    language "C++"
    characterset "ASCII"
    targetdir "./Output"

    files { 
        "./FreeParser/include/**.h",
        "./FreeParser/src/**.cpp"
    }

    includedirs {
        "./Core/include",
        "./FreeParser/include",
        "./FreeParser/depend",
        --"./FreeParser/depend/freetype"
    }

    libdirs {
        "./Output",
        "./FreeParser/depend/"
    }

    filter "configurations:Debug"
        targetname "CrabGui_FreeParser_d"
        links { "CrabGui_Core_d" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        symbols "On"

    filter "configurations:Release"
        links { "CrabGui_Core" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "CRABGUI_DLL" }
        optimize "On"



-------------------------------------------------------------------------------
project "CrabGui_Sample"
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
    }

    libdirs {
        "./Output",
    }

    filter "configurations:Debug"
        targetname "CrabGui_Sample_d"
        links { "CrabGui_Core_d", "CrabGui_FreeParser_d", "CrabGui_FreeParser_d" }
        defines { "WIN32", "_DEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
        symbols "On"

    filter "configurations:Release"
        links { "CrabGui_Core", "CrabGui_FreeParser", "CrabGui_FreeParser" }
        defines { "WIN32", "NDEBUG", "_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
        optimize "On"

