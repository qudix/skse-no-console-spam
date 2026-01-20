includes("lib/commonlibsse")

set_project("skse-no-console-spam")
set_version("1.1.0")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

target("skse-no-console-spam")
    add_rules("commonlibsse.plugin", {
        name = "no-console-spam",
        author = "qudix",
        description = "BEGONE CONSOLE SPAM!"
    })

    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
