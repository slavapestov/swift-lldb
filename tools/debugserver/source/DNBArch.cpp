//===-- DNBArch.cpp ---------------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  Created by Greg Clayton on 6/24/07.
//
//===----------------------------------------------------------------------===//

#include "DNBArch.h"
#include <assert.h>
#include <mach/mach.h>

#include <map>

#include "DNBLog.h"

typedef std::map<uint32_t, DNBArchPluginInfo> CPUPluginInfoMap;

static uint32_t g_current_cpu_type = 0;
CPUPluginInfoMap g_arch_plugins;


static const DNBArchPluginInfo *
GetArchInfo ()
{
    CPUPluginInfoMap::const_iterator pos = g_arch_plugins.find(g_current_cpu_type);
    if (pos != g_arch_plugins.end())
        return &pos->second;
    return NULL;
}


uint32_t
DNBArchProtocol::GetArchitecture ()
{
    return g_current_cpu_type;
}

bool
DNBArchProtocol::SetArchitecture (uint32_t cpu_type)
{
    g_current_cpu_type = cpu_type;
    bool result = g_arch_plugins.find(g_current_cpu_type) != g_arch_plugins.end();
    DNBLogThreadedIf (LOG_PROCESS, "DNBArchProtocol::SetDefaultArchitecture (cpu_type=0x%8.8x) => %i", cpu_type, result);
    return result;
}

void
DNBArchProtocol::RegisterArchPlugin (const DNBArchPluginInfo &arch_info)
{
    if (arch_info.cpu_type)
        g_arch_plugins[arch_info.cpu_type] = arch_info;
}

uint32_t
DNBArchProtocol::GetRegisterCPUType ()
{
    const DNBArchPluginInfo *arch_info = GetArchInfo ();
    if (arch_info)
        return arch_info->cpu_type;
    return 0;
}

const DNBRegisterSetInfo *
DNBArchProtocol::GetRegisterSetInfo (nub_size_t *num_reg_sets)
{
    const DNBArchPluginInfo *arch_info = GetArchInfo ();
    if (arch_info)
        return arch_info->GetRegisterSetInfo (num_reg_sets);
    *num_reg_sets = 0;
    return NULL;
}

DNBArchProtocol *
DNBArchProtocol::Create (MachThread *thread)
{
    const DNBArchPluginInfo *arch_info = GetArchInfo ();
    if (arch_info)
        return arch_info->Create (thread);
    return NULL;

}

const uint8_t *
DNBArchProtocol::GetBreakpointOpcode (nub_size_t byte_size)
{
    const DNBArchPluginInfo *arch_info = GetArchInfo ();
    if (arch_info)
        return arch_info->GetBreakpointOpcode (byte_size);
    return NULL;
}

