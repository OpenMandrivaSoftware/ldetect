#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "libldetect.h"

static int verboze = 0;

static void printit(struct pciusb_entries entries, void print_class(unsigned long )) {
	unsigned int i;
	for (i = 0; i < entries.nb; i++) {
		struct pciusb_entry *e = &entries.entries[i];
		printf("%-16s: ", e->module ? e->module : "unknown");
		if (e->text)
			printf(e->text);
		else	printf("unknown (%04x/%04x/%04x/%04x)", e->vendor, e->device, e->subvendor, e->subdevice);
		if (e->class_id) {
			print_class(e->class_id);
		}
		if (verboze && e->text) {
			printf(" (vendor:%04x device:%04x", e->vendor, e->device);
			if (e->subvendor != 0xffff || e->subdevice != 0xffff)
				printf(" subv:%04x subd:%04x", e->subvendor, e->subdevice);
			printf(")");
		}
		printf("\n");
	}
	pciusb_free(&entries);
}

static void print_pci_class(unsigned long class_id) {
  const char *s = pci_class2text(class_id);
  if (strcmp(s, "NOT_DEFINED") != 0) 
     printf(" [%s]", s);
}

static void print_usb_class(unsigned long class_id) {
  struct usb_class_text s = usb_class2text(class_id);
  if (s.usb_class_text) {
    printf(" [");
    printf("%s", s.usb_class_text);
    if (s.usb_sub_text) printf("|%s", s.usb_sub_text);
    if (s.usb_prot_text) printf("|%s", s.usb_prot_text);
    printf("]");
  }
}

static void print_dmi_entries(struct dmi_entries entries) {
	unsigned int i;
	for (i = 0; i < entries.nb; i++)
		printf("%-16s: %s\n", entries.entries[i].module, entries.entries[i].constraints);
}

int main(int argc, char **argv) {
	char ** ptr = argv;

	while (ptr && *ptr) {
		if (!strcmp(*ptr, "-h") || !strcmp(*ptr, "--help")) {
			printf("usage: lspcidrake [-v|-u]\n"
				"-p <file>: pci devices source [/proc/bus/pci/devices by default]\n"
				"-u <file>: usb devices source [/proc/bus/usb/devices by default]\n"
				"-v : verbose mode [print ids and sub-ids], implies full probe\n"
				"--dmidecode <file>: to use this dmidecode output instead of calling dmidecode\n"
				);	
			return 0;	
		}
		if (!strcmp(*ptr, "-v"))
			verboze = 1;
		else if (!strcmp(*ptr, "-u"))
		  proc_usb_path = *++ptr;
		else if (!strcmp(*ptr, "-p"))
		  proc_pci_path = *++ptr;
		else if (!strcmp(*ptr, "--dmidecode"))
		  dmidecode_file = *++ptr;
		ptr++;
	}

	printit(pci_probe(), print_pci_class);
	printit(usb_probe(), print_usb_class);
	
	if (geteuid() == 0 || dmidecode_file) {
	    struct dmi_entries dmi_entries = dmi_probe();
	    print_dmi_entries(dmi_entries);
	    dmi_entries_free(dmi_entries);
	}

	return 0;
}
