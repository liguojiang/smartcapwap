#include "capwap.h"
#include "capwap_element.h"

/********************************************************************

 0
 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+
|   Name ...
+-+-+-+-+-+-+-+-+

Type:   45 for WTP Name

Length:   >= 1

********************************************************************/

/* */
static void capwap_wtpname_element_create(void* data, capwap_message_elements_handle handle, struct capwap_write_message_elements_ops* func) {
	int length;
	struct capwap_wtpname_element* element = (struct capwap_wtpname_element*)data;

	ASSERT(data != NULL);

	length = strlen((char*)element->name);
	ASSERT(length > 0);

	func->write_block(handle, element->name, length);
}

/* */
static void* capwap_wtpname_element_parsing(capwap_message_elements_handle handle, struct capwap_read_message_elements_ops* func) {
	unsigned short length;
	struct capwap_wtpname_element* data;

	ASSERT(handle != NULL);
	ASSERT(func != NULL);

	length = func->read_ready(handle);
	if ((length < 1) || (length > CAPWAP_WTPNAME_MAXLENGTH)) {
		capwap_logging_debug("Invalid WTP Name element: underbuffer");
		return NULL;
	}

	/* Retrieve data */
	data = (struct capwap_wtpname_element*)capwap_alloc(sizeof(struct capwap_wtpname_element));
	data->name = (uint8_t*)capwap_alloc(length + 1);
	func->read_block(handle, data->name, length);
	data->name[length] = 0;

	return data;
}

/* */
static void capwap_wtpname_element_free(void* data) {
	struct capwap_wtpname_element* element = (struct capwap_wtpname_element*)data;

	ASSERT(data != NULL);

	if (element->name) {
		capwap_free(element->name);
	}

	capwap_free(data);
}

/* */
struct capwap_message_elements_ops capwap_element_wtpname_ops = {
	.create_message_element = capwap_wtpname_element_create,
	.parsing_message_element = capwap_wtpname_element_parsing,
	.free_parsed_message_element = capwap_wtpname_element_free
};
