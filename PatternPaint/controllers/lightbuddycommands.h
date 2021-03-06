#ifndef LIGHTBUDDYCOMMANDS_H
#define LIGHTBUDDYCOMMANDS_H

#include "serialcommand.h"

#include <QByteArray>

namespace LightBuddyCommands {
/// Erase the
SerialCommand eraseFlash();

/// Create a new file, with the given size. The response
/// @param Size of the file, in bytes. Must be smaller than the maximum
/// file size for the controller, or the operation will fail
SerialCommand fileNew(int sizeBytes);
SerialCommand writePage(int sector, int offset, QByteArray data);
SerialCommand reloadAnimations();
}

#endif // LIGHTBUDDYCOMMANDS_H
