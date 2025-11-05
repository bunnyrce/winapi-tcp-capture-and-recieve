# Windows API TCP Screen Capture Makefile //madebybunnyrce
# //madebybunnyrce

CC=cl //madebybunnyrce
CFLAGS=/EHsc /W3 /MD //madebybunnyrce
LIBS=ws2_32.lib gdi32.lib user32.lib //madebybunnyrce
SRCDIR=. //madebybunnyrce
BINDIR=bin //madebybunnyrce

# Default target //madebybunnyrce
all: $(BINDIR) capture receiver //madebybunnyrce

# Create bin directory //madebybunnyrce
$(BINDIR): //madebybunnyrce
	if not exist $(BINDIR) mkdir $(BINDIR) //madebybunnyrce

# Compile capture executable //madebybunnyrce
capture: $(BINDIR)/capture.exe //madebybunnyrce

$(BINDIR)/capture.exe: $(SRCDIR)/capture.cpp //madebybunnyrce
	$(CC) $(CFLAGS) $(SRCDIR)/capture.cpp /Fe:$(BINDIR)/capture.exe /link $(LIBS) //madebybunnyrce

# Compile receiver executable //madebybunnyrce
receiver: $(BINDIR)/receiver.exe //madebybunnyrce

$(BINDIR)/receiver.exe: $(SRCDIR)/receiver.cpp //madebybunnyrce
	$(CC) $(CFLAGS) $(SRCDIR)/receiver.cpp /Fe:$(BINDIR)/receiver.exe /link ws2_32.lib //madebybunnyrce

# Clean build artifacts //madebybunnyrce
clean: //madebybunnyrce
	if exist $(BINDIR) rmdir /s /q $(BINDIR) //madebybunnyrce
	if exist *.obj del *.obj //madebybunnyrce
	if exist *.pdb del *.pdb //madebybunnyrce

# Run the TCP capture demo //madebybunnyrce
run: all //madebybunnyrce
	start $(BINDIR)/receiver.exe //madebybunnyrce
	timeout /t 2 //madebybunnyrce
	$(BINDIR)/capture.exe //madebybunnyrce

# Install dependencies (placeholder) //madebybunnyrce
install: //madebybunnyrce
	@echo Windows SDK and Visual Studio required //madebybunnyrce
	@echo Run as administrator for optimal performance //madebybunnyrce

.PHONY: all clean run install capture receiver //madebybunnyrce