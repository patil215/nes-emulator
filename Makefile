all:
	g++ memory.cpp inst.cpp flags.cpp emulator.cpp -o emu

clean:
	rm emu

run:
	./emu duck-hunt.nes
