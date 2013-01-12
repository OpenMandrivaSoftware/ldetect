#ifndef _LDETECT_HID
#define _LDETECT_HID

#include <iostream>
#include <iomanip>
#include <string>
#include <ostream>
#include <sstream>

#include "libldetect.h"

#pragma GCC visibility push(default)

namespace ldetect {
    class hidEntry {
	public:
	    hidEntry(const std::string &module, const std::string &text) EXPORTED :
		module(module), text(text) {}

	    std::string module;
	    std::string text;

	    friend std::ostream& operator<<(std::ostream& os, const hidEntry& e) EXPORTED;
    };

    class hid {
	public:
	    hid() EXPORTED :
		_entries() {}

	    const hidEntry& operator[] (uint16_t i) const EXPORTED{
		return _entries[i];
	    }

	    operator bool() const throw() EXPORTED {
		return !_entries.empty();
	    }

	    bool operator! () const throw() EXPORTED {
		return _entries.empty();
	    }

	    uint16_t size() const throw() EXPORTED {
		return _entries.size();
	    }

	    void probe(void) EXPORTED;

	private:
	    std::vector<hidEntry> _entries;
    };
}

#pragma GCC visibility pop

#endif
