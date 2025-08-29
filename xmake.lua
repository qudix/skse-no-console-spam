-- set minimum xmake version
set_xmakever("2.9.0")

-- includes
includes("lib/commonlibsse")

-- set project
set_project("skse-no-console-spam")
set_version("1.1.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)

-- targets
target("skse-no-console-spam")
    -- add dependencies to target
    add_deps("commonlibsse")

    -- add commonlibsse plugin
    add_rules("commonlibsse.plugin", {
        name = "no-console-spam",
        author = "qudix",
        description = "BEGONE CONSOLE SPAM!"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
