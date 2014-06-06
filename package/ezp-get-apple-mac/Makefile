include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-get-apple-mac
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-get-apple-mac
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=TEST
  DEPENDS:=ezp-nvram
endef

define Package/ezp-get-apple-mac/description
	Command Line Interface.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. "

endef

define Package/ezp-get-apple-mac/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mac-sense $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-get-apple-mac))
