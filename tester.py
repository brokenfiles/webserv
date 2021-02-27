# Tester dedicated to the 42 project webserv.
# 04/11/2020
# Reproduction of the "official" tester given in the project page (named "tester" or "ubuntu_tester")
# Thanks Wireshark. If you need the networks captures, feel free to ask. I have kept them
# For info, it seems that the tester only call uppercase on its input and return it

# ---------------------------------------------------------------------------
# ---------------------------- 1. CHECKS ------------------------------------
# ---------------------------------------------------------------------------

import os
import sys
import json
import math
import requests

from requests.auth import HTTPBasicAuth
from sys import platform

global verbose
verbose = 0

if (len(sys.argv) == 3 and sys.argv[2] == '-v'):
    verbose = 1
if (os.system('lsof -c webserv > /dev/null') != 0):
    print("Webserv is not running")
    exit()

# ---------------------------------------------------------------------------
# ----------------------------- 2. CORE -------------------------------------
# ---------------------------------------------------------------------------

class assertTypes:
    BODY_CONTAIN_ASSERT = 1
    FILE_CONTAIN_ASSERT = 2
    RES_HD_CONTAIN_ASSERT = 3
    RES_BODY_SIZE_ASSERT = 4

class bcolors:
    HEADER = '\033[95m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def printHdReqRes(r):
    print()
    indent = "          Request >  "
    for idA, reqKey in enumerate(r.request.headers):
        print(indent + reqKey + ": " + r.request.headers[reqKey])
    print()
    indent = "          Response > "
    for idB, resKey in enumerate(r.headers):
        print(indent + resKey + ": " + r.headers[resKey])
    print()

def bodyContain(r, str):
    if (str in r.text): return True
    else: return False

def fileContain(r, strLookingFor, path):
    file = open(path, 'r').read()
    if (strLookingFor == file): return True
    else: return False

def resHeadersKeyVal(r, hdKeyTab = [], hdValTab = []):
    validated = 0
    for idA, resKey in enumerate(r.headers):
        for idB, wantKey in enumerate(hdKeyTab):
            if (resKey == wantKey):
                if (r.headers[resKey] == hdValTab[idB]):
                    validated += 1
    if (validated == len(hdKeyTab)):
        return True
    return False

def resBodySize(r, size):
    if (r.text and str(len(r.text)) >= size and str(r.headers["Content-Length"]) == size): return True
    else: return False

def moreAsserts(r, assertLevel, *args):
    indexArgsUsed = 0
    ret = False
    if (assertTypes.BODY_CONTAIN_ASSERT in assertLevel and bodyContain(r, args[0][indexArgsUsed]) == True):
        ret = True
        indexArgsUsed += 1
    if (assertTypes.FILE_CONTAIN_ASSERT in assertLevel and fileContain(r, args[0][indexArgsUsed], args[0][indexArgsUsed + 1]) == True):
        ret = True
        indexArgsUsed += 2
    if (assertTypes.RES_HD_CONTAIN_ASSERT in assertLevel and resHeadersKeyVal(r, args[0][indexArgsUsed], args[0][indexArgsUsed + 1]) == True):
        ret = True
        indexArgsUsed += 2
    if (assertTypes.RES_BODY_SIZE_ASSERT in assertLevel and resBodySize(r, args[0][indexArgsUsed]) == True ):
        ret = True
        indexArgsUsed += 1

    return ret

def assertResponse(r, code, index, assertLevel = [], *args):
    ret = False
    if (len(assertLevel)): ret = moreAsserts(r, assertLevel, args)
    else: ret = True
    if (ret and r.status_code == code):
        info = bcolors.OKGREEN + "OK" + bcolors.ENDC + " - " + str(r.status_code)
    else:
        info = bcolors.FAIL + "KO" + bcolors.ENDC + " - " + str(r.status_code) + " - Should have been received : " + str(code)
    url = "           • #" + str(index).ljust(2, ' ') + " : " + str(r.request.method) + " "
    if (len(r.request.url) > 60):
        url += r.request.url[16:60] + " [..." + str(len(r.request.url)) + "]"
    else: url += str(r.request.url)[16:]
    url = str(url).ljust(80, ' ')

    bodyInfo = "REQ | RES BODY = "
    if r.request.body:
        bodyInfo += (str(len(r.request.body)) + " | ")
    else:
        bodyInfo += "Empty | "

    if r.text:
        bodyInfo += str(len(r.text))
    else:
        bodyInfo += "Empty"
    print(url + "   =   " + info + "    ==>    " + bodyInfo)
    if verbose == 1:
        printHdReqRes(r)

def run(sys):

    print("\n       Platform = " + platform)
    if (len(sys.argv) == 1):
        TESTS_42()
    elif (len(sys.argv) >= 2):
        TESTS_42(sys.argv[1])
    print()

# -----------------------------------------------------------------------------
# -------------------------------- 3. TESTS -----------------------------------
# -----------------------------------------------------------------------------

def TESTS_42(testNum = 0):

    index = 0
    follow = False
    chainUp = False

    if (testNum != 0 and testNum.endswith('+')):
        testNum = int(testNum[:-1])
        follow = True

    print("\n     ~ 42 TESTS ------------------------> \n")



    # -----------------------------------------------------------------------------> #1 - STAGE 1

    # GET / HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/", headers=hd)
        assertResponse(r, 200, index)

    # -----------------------------------------------------------------------------> #2 - STAGE 1

    # POST / HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Content-Type: test/file
    # Accepted-Encoding: gzip

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Transfer-Encoding": "chunked",
            "Content-Type": "test/file",
            "Accepted-Encoding": "gzip"
        }
        payload = "0\r\n\r\n" # --> Need confirmation about Request Python Library
        # The last \n seems to be send in another request ... idk why, may be to trool us but it may raise an error of invalid request
        r = requests.post("http://localhost:8888/", headers=hd, data=payload)
        assertResponse(r, 405, index, [assertTypes.RES_BODY_SIZE_ASSERT], "212")

    # -----------------------------------------------------------------------------> #3 - STAGE 1

    # HEAD / HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1"
        }
        r = requests.head("http://localhost:8888/", headers=hd)
        assertResponse(r, 405, index)

    # ----------------------------------------------------------------------------- #4 - STAGE 1

    # GET /directory HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return ./youpi.bad_extension

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #5 - STAGE 1

    # GET /directory/youpi.bad_extension HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return ./youpi.bad_extension
    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/youpi.bad_extension", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #6 - STAGE 1

    # GET /directory/youpi.bla HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/youpi.bla", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #7 - STAGE 1

    # GET /directory/oulalala HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return Error

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/oulalala", headers=hd)
        assertResponse(r, 404, index)

    # ----------------------------------------------------------------------------- #8 - STAGE 1


    # GET /directory/nop HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return youpi.bad_extension

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/nop", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #9 - STAGE 1

    # The only difference here is the "/" at the end of the URL

    # GET /directory/nop/ HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return youpi.bad_extension

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/nop/", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #10 - STAGE 1

    # GET /directory/nop/other.pouic HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return other.pouic

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/nop/other.pouic", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #11 - STAGE 1

    # GET /directory/nop/other.pouac HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return Error 404

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/nop/other.pouac", headers=hd)
        assertResponse(r, 404, index)

    # ----------------------------------------------------------------------------- #12 - STAGE 1

    # GET /directory/Yeah HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    # Should return Error 404

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/Yeah", headers=hd)
        assertResponse(r, 404, index)

    # ----------------------------------------------------------------------------- #13 - STAGE 1

    # GET /directory/Yeah/not_happy.bad_extension HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Accept-Encoding: gzip

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip"
        }
        r = requests.get("http://localhost:8888/directory/Yeah/not_happy.bad_extension", headers=hd)
        assertResponse(r, 200, index)

    # ----------------------------------------------------------------------------- #14 - STAGE 1 -  PUT 1.000 BYTES

    # PUT /put_test/file_should_exist_after HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Accepted-Encoding: gzip

    # The last \n seems to be send in another request ... idk why, may be to trool.
    # It may raise an error of invalid request

    # 1000 =    3e8 en HEX

    # File should exist after with a size of 1000
    # Response Code must be set to 201
    # Content-Length to 0
    # Content-Type to text/plain

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        if os.path.exists("www/test42/file_should_exist_after"): os.remove("www/test42/file_should_exist_after")
        payload = "3e8\r\n"
        payload += "e" * 1000
        payload += "\r\n"
        payload += "0\r\n\r\n"
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Transfer-Encoding": "chunked"
        }
        r = requests.put("http://localhost:8888/put_test/file_should_exist_after", headers=hd, data=payload)
        assertResponse(r, 201, index)

    # ----------------------------------------------------------------------------- #15 - STAGE 1 - PUT 10.000.000 BYTES

    # PUT /put_test/file_should_exist_after HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Accepted-Encoding: gzip

    # 32768 =     8 000 en HEX
    # 186A0 =   100 000 en HEX
    # F4240 = 1 000 000 en HEX

    # In this test, body is sent by chunk of 32 768 bytes or 8000 in hexa
    # File should STILL BE THERE but this time with a size of 10 000 000 bytes

    # Response Code must be 200
    # Content-Length to 0

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Transfer-Encoding": "chunked"
        }
        i = 10000000 // 32768
        i = math.floor(i)
        mod = 10000000 % 32768 # == 5760 (dec) or 1680 (hex)
        payload = ""
        for x in range(0, i):
            payload += "8000\r\n"
            payload += "z" * 32768
            payload += "\r\n"
        payload += "1680"
        payload += "\r\n"
        payload += "z" * mod
        payload += "\r\n"
        payload += "0\r\n\r\n"

        # The last \n seems to be send in another request ... idk why, may be to trool. It may raise an error of invalid request
        r = requests.put("http://localhost:8888/put_test/file_should_exist_after", headers=hd, data=payload)
        assertResponse(r, 200, index)
        # No longer needed :
        if os.path.exists("www/test42/file_should_exist_after"): os.remove("www/test42/file_should_exist_after")

    # ----------------------------------------------------------------------------- #16 - STAGE 2 - POST 100.000.000 BYTES

    # POST /directory/youpi.bla HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Accepted-Encoding: gzip

    # 8000 =     32 768 en HEX
    # 186A0 =   100 000 en HEX
    # F4240 = 1 000 000 en HEX

    # Also, in hexadecimal, 8000 is written : 38 30 30 30 (usefull for Wireshark)
    # In this test too, body is sent by chunk of 32 768 bytes or 8000 in hexa
    # File should exist after with a size of 100 000 000

    # Response Code must be 200

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked"
        }
        i = 100000000 // 32768
        i = math.floor(i)
        mod = 100000000 % 32768 # == 5760 (dec) or 1680 (hex)
        payload = ""
        for x in range(0, i):
            payload += "8000\r\n"
            payload += "n" * 32768
            payload += "\r\n"
        payload += "1680"
        payload += "\r\n"
        payload += "n" * mod
        payload += "\r\n"
        payload += "0\r\n\r\n"
        r = requests.post("http://localhost:8888/directory/youpi.bla", headers=hd, data=payload)
        assertResponse(r, 200, index, [assertTypes.RES_BODY_SIZE_ASSERT], "100000000")


    # ----------------------------------------------------------------------------- #17 - STAGE 2 - POST 100.000.000 BYTES

    # POST /directory/youpla.bla HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Content-Type: test/file
    # Accepted-Encoding: gzip

    # 8000  =    32 768 en HEX
    # 186A0 =   100 000 en HEX
    # F4240 = 1 000 000 en HEX

    # Also, in hexadecimal, 8000 is written : 38 30 30 30 (usefull for Wireshark)
    # In this test too, body is sent by chunk of 32 768 bytes or 8000 in hexa
    # File should exist after with a size of 100 000 000

    # Response Code must be 200

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked"
        }
        i = 100000000 // 32768
        i = math.floor(i)
        mod = 100000000 % 32768 # == 5760 (dec) or 1680 (hex)
        payload = ""
        for x in range(0, i):
            payload += "8000\r\n"
            payload += "c" * 32768
            payload += "\r\n"
        payload += "1680"
        payload += "\r\n"
        payload += "c" * mod
        payload += "\r\n"
        payload += "0\r\n\r\n"
        r = requests.post("http://localhost:8888/directory/youpla.bla", headers=hd, data=payload)
        assertResponse(r, 200, index, [assertTypes.RES_BODY_SIZE_ASSERT], "100000000")


    # ----------------------------------------------------------------------------- #18 - STAGE 2 - POST 100.000 BYTES - Special Headers

    # POST /directory/youpi.bla HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Content-Type: test/file
    # Accepted-Encoding: gzip
    # X-Secret-Header-For-Test: 1

    # 8000  =    32 768 en HEX
    # 186A0 =   100 000 en HEX
    # F4240 = 1 000 000 en HEX

    # Also, in hexadecimal, 8000 is written : 38 30 30 30 (usefull for Wireshark)
    # In this test too, body is sent by chunk of 32 768 bytes or 8000 in hexa
    # File should exist after with a size of 100 000 000

    # POST /directoru/youpi.bla HTTP/1.1
    # Host: localhost:8080
    # User-Agent: Go-http-client/1.1
    # Transfer-Encoding: chunked
    # Content-Type: test/file
    # Accepted-Encoding: gzip
    # Response Code must be 200

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked",
            "X-Secret-Header-For-Test": "1"
        }
        i = 100000 // 32768
        i = math.floor(i)
        mod = 100000 % 32768 # == 5760 (dec) or 1680 (hex)
        payload = ""
        for x in range(0, i):
            payload += "8000\r\n"
            payload += "y" * 32768
            payload += "\r\n"
        payload += "1680"
        payload += "\r\n"
        payload += "y" * mod
        payload += "\r\n"
        payload += "0\r\n\r\n"
        r = requests.post("http://localhost:8888/directory/youpi.bla", headers=hd, data=payload)
        assertResponse(r, 200, index, [assertTypes.RES_BODY_SIZE_ASSERT], "100000")


    # ----------------------------------------------------------------------------- #19 - STAGE 3 - Small POST (1)

    # http://localhost:8080/post_body with a size of 0

    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked",
        }
        payload = "0\r\n\r\n"
        r = requests.post("http://localhost:8888/post_body", headers=hd, data=payload)
        assertResponse(r, 200, index, [assertTypes.RES_BODY_SIZE_ASSERT], "71")
        # Here, set the 71 to the Content-Length of your response, since the purpose in this test is to
        # verify that you send an answer DESCRIBING the resulting changes of the request like "Successfull POST request" (size = 24)


    # ----------------------------------------------------------------------------- #20 - STAGE 3 - Small POST (2)

    # http://localhost:8080/post_body with a size of 100
    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked",
        }
        payload = "64\r\n"
        payload += "t" * 100
        payload += "0\r\n\r\n"
        r = requests.post("http://localhost:8888/post_body", headers=hd, data=payload)
        assertResponse(r, 200, index, [assertTypes.RES_BODY_SIZE_ASSERT], "71")

    # ----------------------------------------------------------------------------- #21 - STAGE 3 - Small POST (3)

    # http://localhost:8080/post_body with a size of 200
    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked",
        }
        payload = "c8\r\n"
        payload += "c" * 200
        payload += "0\r\n\r\n"
        r = requests.post("http://localhost:8888/post_body", headers=hd, data=payload)
        assertResponse(r, 413, index)

    # ----------------------------------------------------------------------------- #22 - STAGE 3 - Small POST (4)

    # http://localhost:8080/post_body with a size of 101

    # http://localhost:8080/post_body with a size of 200
    index += 1
    if (follow == True and testNum != 0 and index >= testNum):
        chainUp = True
    if (chainUp or testNum == 0 or index == int(testNum)):
        hd = {
            "Host": "localhost:8080",
            "User-Agent": "Go-http-client/1.1",
            "Accept-Encoding": "gzip",
            "Content-Type": "test/file",
            "Transfer-Encoding": "chunked",
        }
        payload = "65\r\n"
        payload += "u" * 101
        payload += "0\r\n\r\n"
        r = requests.post("http://localhost:8888/post_body", headers=hd, data=payload)
        assertResponse(r, 413, index)





# -----------------------------------------------------------------------------
# ----------------------------------- MAIN ------------------------------------
# -----------------------------------------------------------------------------

run(sys)

# -----------------------------------------------------------------------------
# ------------------------------ WEBSERV CONFIG -------------------------------
# -----------------------------------------------------------------------------


# server {

# 	listen 	8888
# 	server_name     test42
# 	error   	    www/errors

# 	location / {
#         method      GET
# 		root 	    ./www/test42
#         index       index.html
#         ext			.bla
# 		cgi         ./www/cgi-bin/cgi_tester
# 	}

#     location /directory {
# 		method		GET,POST
# 		root 		./www/test42/YoupiBanane
# 		index 		youpi.bad_extension
# 		ext 		.bla
# 		cgi 		./www/cgi-bin/cgi_tester
# 	}

#     location /put_test {
#         method		PUT
#         root       ./www/test42/
#     }

#     location /post_body {
# 		method   	POST
# 		root 		./www/test42
# 		index 		index.html
# 		max_body 	100
# 		ext			.bla

# 	}
# }