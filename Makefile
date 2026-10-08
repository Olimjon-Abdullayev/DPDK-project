all:meson.build
	rm -rf build
	meson setup build
	ninja -C build
	
run: ./build/mini_project_01
	sudo ./build/mini_project_01 -l 0-3 -n 4 -- -q 1 -p 0x3 -P --no-mac-updating

run_gdb:./build/mini_project_01
	sudo gdb --args ./build/mini_project_01 -l 0-3 -n 4 -- -q 1 -p 0x3 -P --no-mac-updating

clean: 
	rm -rf ./build