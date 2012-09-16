///////////////////////////////////////////////////////////////////////////////
// BOSSA
//
// Copyright (c) 2011-2012, ShumaTech
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the <organization> nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///////////////////////////////////////////////////////////////////////////////
#include "OSXPortFactory.h"
#include "PosixSerialPort.h"

#include <string.h>
#include <stdio.h>

#include <string>

OSXPortFactory::OSXPortFactory()
{
    _dir = opendir("/dev");
}

OSXPortFactory::~OSXPortFactory()
{
    if (_dir)
        closedir(_dir);
}

SerialPort::Ptr
OSXPortFactory::create(const std::string& name)
{
    bool isUsb = false;

    if (name.find("usb") != std::string::npos)
        isUsb = true;

    return SerialPort::Ptr(new PosixSerialPort(name, isUsb));
}

std::string
OSXPortFactory::begin()
{
    if (!_dir)
        return end();

    rewinddir(_dir);

    return next();
}

std::string
OSXPortFactory::next()
{
    struct dirent* entry;

    if (!_dir)
        return end();

    while ((entry = readdir(_dir)))
    {
        if (strncmp("cu.", entry->d_name, sizeof("cu.") - 1) == 0)
            return std::string(entry->d_name);
    }

    return end();
}

std::string
OSXPortFactory::end()
{
    return std::string();
}
