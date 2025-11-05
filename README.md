# WinAPI TCP Screen Capture - Persistent Connection Beast
<!-- //madebybunnyrce -->

Native C++ screen capture using WinAPI calls (GetDC, BitBlt) with TCP streaming. This shit maintains persistent connections that are easier to monitor than UDP packets. Creates continuous TCP streams that any network monitor will notice. Perfect for testing how connection-based capture works.

## What This TCP Sh*t Does

- **Screen Capture**: Uses obvious WinAPI calls like `BitBlt()` and `GetDC()` 
- **TCP Streaming**: Maintains persistent connections that are easy as f*** to detect
- **No Stealth**: Even more obvious than UDP - continuous TCP streams
- **Human Language**: Comments that sound like actual people wrote them, not f***ing robots

## Files in This TCP Mess

- `capture.cpp` - Main TCP screen capture executable (obvious connection patterns)
- `receiver.cpp` - TCP server for streamed frames
- `build.bat` - Batch script to compile this networking garbage
- `config.ini` - Configuration file with obvious TCP settings
- `README.md` - This file explaining the obvious sh*t

## How to Use This TCP Crap

1. **Compile the bullsh*t:**
   ```cmd
   build.bat
   ```

2. **Run the receiver first:**
   ```cmd
   receiver.exe
   ```

3. **Start the capture:**
   ```cmd
   capture.exe
   ```

4. **Watch network monitors detect persistent connections** - even more obvious than UDP

## Why TCP is Even More Obvious

This isn't meant to be a real cheat, you dumb***. It's for **testing anti-cheat detection** of TCP-based communication:

- **Persistent TCP connections** - easier to monitor than UDP bursts
- **Connection establishment patterns** - obvious three-way handshake
- **Stream-based data flow** - consistent bandwidth usage
- **Connection state tracking** - anti-cheat can monitor connection lifecycle
- **Higher overhead** - more network metadata to analyze

## Detection Signatures

Anti-cheat systems should easily catch:
- TCP socket creation and bind() calls
- listen() and accept() server patterns
- connect() client connection attempts
- send()/recv() continuous streaming
- Consistent data flow patterns
- Screen capture API usage combined with networking

## Requirements

- Windows 10/11 (duh, it's WinAPI)
- Visual Studio Build Tools or full VS
- Admin privileges (because this sh*t needs low-level access)
- A sense of humor about how obvious persistent connections are

## Limitations

- **Zero stealth** - designed to be caught
- **Persistent connections** - easier to detect than UDP
- **No encryption** - raw bitmap data over TCP
- **Fixed streaming** - predictable data flow patterns
- **Connection overhead** - more obvious than connectionless UDP

## Legal Disclaimer

This is for **educational and testing purposes only**. Don't use this sh*t to actually cheat in games with TCP connections - you'll get caught even faster than UDP. We're not responsible if you're dumb enough to use obviously detectable test code in real scenarios.

---

**Remember**: TCP connections are even more obvious than UDP packets. This maintains persistent network connections that are trivial for anti-cheat systems to monitor and analyze.