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
