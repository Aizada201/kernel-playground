#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x14\x00\x00\x00\xa3\x75\xe5\x88"
	"single_open\0"
	"\x14\x00\x00\x00\xcb\xc0\x25\x23"
	"seq_printf\0\0"
	"\x14\x00\x00\x00\xe2\xbe\x85\x4e"
	"seq_write\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x20\x00\x00\x00\x3d\x8a\xf5\xc3"
	"nf_unregister_net_hook\0\0"
	"\x1c\x00\x00\x00\xac\x98\x79\xc9"
	"remove_proc_entry\0\0\0"
	"\x14\x00\x00\x00\xb8\x4d\xa6\xfc"
	"seq_read\0\0\0\0"
	"\x14\x00\x00\x00\xa2\xe6\x82\x14"
	"seq_lseek\0\0\0"
	"\x18\x00\x00\x00\x30\xf2\x77\x80"
	"single_release\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x28\x00\x00\x00\xb3\x1c\xa2\x87"
	"__ubsan_handle_out_of_bounds\0\0\0\0"
	"\x14\x00\x00\x00\xb6\x14\x2f\x32"
	"init_net\0\0\0\0"
	"\x20\x00\x00\x00\xdc\xa4\xc9\x5c"
	"nf_register_net_hook\0\0\0\0"
	"\x14\x00\x00\x00\x85\xf8\xb3\x22"
	"proc_create\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x18\x00\x00\x00\x34\x61\x23\x68"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7BD322A2684A19D1F14E7CA");
