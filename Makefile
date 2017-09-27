include $(TOPDIR)/rules.mk

PKG_NAME:=seread
PKG_VERSION:=v1.0.0
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=base
	CATEGORY:=Nexfi
	TITLE:=serial commutication
endef

	#$(CP) ./src/* $(PKG_BUILD_DIR)/
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/seread.init $(1)/etc/init.d/seread
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
