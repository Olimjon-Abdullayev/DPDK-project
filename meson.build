project(
    'mini_project_01',
    'c',
    version: '0.1.0',
    default_options: [
        'buildtype=release',
        'c_std=c11',
        'warning_level=2',
    ],
)

dpdk_dep = dependency('libdpdk', required: true)
add_project_arguments('-D_GNU_SOURCE', language: 'c')

src = ['main.c', 'filter.c'] # ---> yangi faylni qo'shish

executable(
    'mini_project_01',
    sources: src,
    dependencies: [dpdk_dep],
    link_args: ['-lm'],
    install: true,
)