all:
	g++ emulator.cpp -o emu

clean:
	rm emu

run:
	./emu donkey-kong.nes
