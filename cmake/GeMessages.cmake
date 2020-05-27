string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(ColorRed   "${Esc}[31m")
set(ColorBoldRed   "${Esc}[31;1m")

function(error message)
    message(SEND_ERROR " ${ColorBoldRed}${message}${ColorReset}")
endfunction()

function(require message)
    if(NOT (${ARGN}))
        error(${message})
    endif()
endfunction()
