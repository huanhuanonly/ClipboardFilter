/**
 * @file Logger.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Record global log
 * 
 * @ingroup huanhuan
 * 
 * @include
 *     @namespace huanhuan
 *         @class    Logger
 *         @variable slog
 */

#include "Logger.h"

namespace huanhuan
{

Logger slog(SETTINGS__LOGGER_MAXCOUNT);

void endl(Logger& er)
{
    er._M_addFlags(Logger::linebreak);
}

void backline(Logger& er)
{
    er.latest().text.clear();
}

} // namespace huanhuan
