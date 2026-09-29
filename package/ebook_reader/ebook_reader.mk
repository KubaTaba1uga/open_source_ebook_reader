################################################################################
#
# Ebook reader app
#
################################################################################

EBOOK_READER_VERSION = v0.0.1
EBOOK_READER_SITE = $(BR2_EXTERNAL_EBK_READER_PATH)/package/ebook_reader
EBOOK_READER_SITE_METHOD = local
EBOOK_READER_DEPENDENCIES = it8951_epaper lvgl host-pkgconf
EBOOK_READER_INSTALL_STAGING = YES
EBOOK_READER_INSTALL_TARGET = YES
# EBOOK_READER_CONF_OPTS += -Dbuildtype=debug -Db_sanitize=address,undefined -Db_lundef=false

$(eval $(meson-package))

