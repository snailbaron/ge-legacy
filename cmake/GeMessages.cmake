string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(ColorRed   "${Esc}[31m")
set(ColorBoldRed   "${Esc}[31;1m")
set(ColorBoldGreen "${Esc}[32;1m")

function(ge_error message)
    message(SEND_ERROR "${ColorBoldRed}[GE] ${message}${ColorReset}")
endfunction()

function(ge_log message)
    message(STATUS "${ColorBoldGreen}[GE] ${message}${ColorReset}")
endfunction()
