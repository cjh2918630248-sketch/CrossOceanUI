# Makefile for CrossOceanUI project

# ============================== variables ==============================
VERSION        ?= v0.0.1
AUTHOR         ?= Jinghui Liang
SINCE          ?= 2026-03-21 10:30
UNTIL          ?= 2026-03-22 17:30
SUBMODULE_URL  ?= ""
SUBMODULE_PATH ?= ""

NPROC          := $(shell nproc)
QT_VERSION     ?= 5.15.2
QT_QMAKE       ?= /opt/Qt/$(QT_VERSION)/gcc_64/bin/qmake
QTMQTT_SRC     := 3rdparty/qtmqtt
QTMQTT_BUILD   := $(QTMQTT_SRC)/build
QTMQTT_INSTALL := $(QTMQTT_SRC)/install

BUILD_DEBUG    := build/debug
BUILD_RELEASE  := build/release

QT_DIR         := /opt/Qt/$(QT_VERSION)/gcc_64
QT_PLUGINS     := $(QT_DIR)/plugins

DEPLOY_DEBUG   := deploy/debug
DEPLOY_RELEASE := deploy/release

# 要部署的二进制文件路径（相对于项目根目录）
# 可覆盖，例如：make deploy-debug DEPLOY_BIN=bin/examples/pointslistmodel/example_pointslistmodel
DEPLOY_BIN     ?= bin/examples/mqtt/example_mqtt

# ============================== phony ==============================
.PHONY: init proto \
	submodule-update submodule-add submodule-remove \
	qtmqtt-build qtmqtt-install qtmqtt-clean \
	build-debug build-release build-clean \
	deploy-debug deploy-release deploy-clean \
	doc-new \
	git-stats-commits git-stats-c++ git-stats-md \
	help

# ============================== init ==============================

# 项目初始化
init: submodule-update proto

# 从 proto/mcu/ 生成 C++ protobuf 代码到 pb/mcu/
proto:

# ============================== submodule ==============================

# 拉取/更新所有 git 子模块
submodule-update:
	git submodule update --init --recursive

# 添加git submodule
submodule-add:
	@echo "Adding submodule: $(SUBMODULE_URL) at path: $(SUBMODULE_PATH)"
	git submodule add $(SUBMODULE_URL) $(SUBMODULE_PATH)
	git submodule update --init --recursive
	@echo "Submodule added. Don't forget to commit the changes."

# 移除git submodule
submodule-remove:
	@echo "Removing submodule: $(SUBMODULE_PATH)"
	git submodule deinit -f -- $(SUBMODULE_PATH)
	rm -rf .git/modules/$(SUBMODULE_PATH)
	git rm -f $(SUBMODULE_PATH)
	@echo "Submodule $(SUBMODULE_PATH) removed. Don't forget to commit the changes."

# ============================== 3rdparty ==============================

# 编译 qtmqtt（shadow build）
qtmqtt-build:
	@mkdir -p $(QTMQTT_BUILD)
	cd $(QTMQTT_BUILD) && $(QT_QMAKE) $(CURDIR)/$(QTMQTT_SRC)/qtmqtt.pro
	$(MAKE) -C $(QTMQTT_BUILD)

# 编译并安装 qtmqtt 到 3rdparty/qtmqtt/intall/
qtmqtt-install: qtmqtt-build
	@mkdir -p $(QTMQTT_INSTALL)/.staging
	$(MAKE) -C $(QTMQTT_BUILD) INSTALL_ROOT=$(CURDIR)/$(QTMQTT_INSTALL)/.staging install
	@mkdir -p $(QTMQTT_INSTALL)/lib $(QTMQTT_INSTALL)/include
	find $(QTMQTT_INSTALL)/.staging -name "libQt5Mqtt*" -exec cp -P {} $(QTMQTT_INSTALL)/lib/ \;
	find $(QTMQTT_INSTALL)/.staging -type d -name "QtMqtt" -exec cp -r {} $(QTMQTT_INSTALL)/include/ \;
	@rm -rf $(QTMQTT_INSTALL)/.staging
	@echo "QtMqtt installed to $(QTMQTT_INSTALL)"

# 清理 qtmqtt 编译产物和安装目录
qtmqtt-clean:
	rm -rf $(QTMQTT_BUILD) $(QTMQTT_INSTALL)

# ============================== build ==============================

# 编译主项目（debug 模式）
build-debug:
	@mkdir -p $(BUILD_DEBUG)
	cd $(BUILD_DEBUG) && $(QT_QMAKE) $(CURDIR)/CrossOceanUI.pro CONFIG+=debug
	$(MAKE) -C $(BUILD_DEBUG) -j$(NPROC)

# 编译主项目（release 模式）
build-release:
	@mkdir -p $(BUILD_RELEASE)
	cd $(BUILD_RELEASE) && $(QT_QMAKE) $(CURDIR)/CrossOceanUI.pro CONFIG+=release
	$(MAKE) -C $(BUILD_RELEASE) -j$(NPROC)

# 清理主项目编译产物
build-clean:
	rm -rf build/

# ============================== deploy ==============================

# 定义通用打包函数（用 $(call _deploy_app,BINARY,DEPLOY_DIR) 调用）
define _deploy_app
	@mkdir -p $(2)/lib $(2)/plugins/platforms
	cp $(1) $(2)/
	cp -P $(QTMQTT_INSTALL)/lib/libQt5Mqtt.so.* $(2)/lib/
	ldd $(1) | awk '/\/opt\/Qt\// {print $$3}' | xargs -I{} cp -Lf {} $(2)/lib/
	ldd $(QTMQTT_INSTALL)/lib/libQt5Mqtt.so.5 | awk '/\/opt\/Qt\// {print $$3}' | xargs -I{} cp -Lf {} $(2)/lib/
	cp $(QT_PLUGINS)/platforms/libqxcb.so $(2)/plugins/platforms/
	ldd $(2)/plugins/platforms/libqxcb.so | awk '/\/lib\// && !/not found/ && !/linux-vdso/ && !/ld-linux/ {print $$3}' | xargs -I{} cp -Lf {} $(2)/lib/ 2>/dev/null; true
	@printf '#!/bin/bash\ncd "$$(dirname "$$0")"\nLD_LIBRARY_PATH=./lib ./$(notdir $(1)) "$$@"\n' > $(2)/run.sh
	@chmod +x $(2)/run.sh
	@echo "Deployed to $(2) — run with: $(2)/run.sh"
endef

# 打包 debug 构建（含 Qt 库、第三方库、平台插件）
deploy-debug: build-debug deploy-clean
	$(call _deploy_app,$(DEPLOY_BIN),$(DEPLOY_DEBUG))

# 打包 release 构建（含 Qt 库、第三方库、平台插件）
deploy-release: build-release deploy-clean
	$(call _deploy_app,$(DEPLOY_BIN),$(DEPLOY_RELEASE))

# 清理打包产物
deploy-clean:
	rm -rf deploy/

# ============================== doc ==============================

# 创建新版本的 docs 目录结构
doc-new:
	@DOC_DIR=docs/$(VERSION); \
	if [ -d "$$DOC_DIR" ]; then \
		echo "Directory $$DOC_DIR already exists, skipping."; \
	else \
		mkdir -p $$DOC_DIR/issues $$DOC_DIR/fix $$DOC_DIR/plan; \
		echo "Created docs structure:"; \
		echo "  $$DOC_DIR/issues/"; \
		echo "  $$DOC_DIR/fix/"; \
		echo "  $$DOC_DIR/plan/"; \
	fi

# ============================== git ==============================

# 按作者与时间段统计C++代码变更行数（新增/删除/净变更）
git-stats-c++:
	@git log \
		--author="$(AUTHOR)" \
		--after="$(SINCE)" \
		--before="$(UNTIL)" \
		--numstat \
		--pretty=format: \
	| grep -E '\.(cpp|h|cc|cxx|hpp)$$' \
	| awk 'BEGIN{a=0;d=0} {a+=$$1; d+=$$2} \
		END{print "Author:  $(AUTHOR)\nSince:   $(SINCE)\nUntil:   $(UNTIL)\n---\nAdded:   " a "\nDeleted: " d "\nNet:     " a-d}'

# 按作者与时间段统计 Markdown 文档变更行数
git-stats-md:
	@git log \
		--author="$(AUTHOR)" \
		--after="$(SINCE)" \
		--before="$(UNTIL)" \
		--numstat \
		--pretty=format: \
	| grep -E '\.md$$' \
	| awk 'BEGIN{a=0;d=0} {a+=$$1; d+=$$2} \
		END{print "Author:  $(AUTHOR)\nSince:   $(SINCE)\nUntil:   $(UNTIL)\n---\nAdded:   " a "\nDeleted: " d "\nNet:     " a-d}'

# 按作者与时间段统计提交数
git-stats-commits:
	@git log \
		--author="$(AUTHOR)" \
		--after="$(SINCE)" \
		--before="$(UNTIL)" \
		--oneline \
	| wc -l \
	| awk '{print "Author:  $(AUTHOR)\nSince:   $(SINCE)\nUntil:   $(UNTIL)\n---\nCommits: " $$1}'

# ============================== help ==============================
# 显示帮助信息
help:
	@echo "Usage: make [target] [VAR=value]"
	@echo ""
	@echo "Variables:"
	@echo "  VERSION            版本号，默认 $(VERSION)"
	@echo "  SERVICE            服务名，用于 docker-build / docker-start / docker-stop / docker-logs"
	@echo "  AUTHOR             提交作者，用于 git-stats-*，默认 $(AUTHOR)"
	@echo "  SINCE              统计起始时间，用于 git-stats-*，默认 $(SINCE)"
	@echo "  UNTIL              统计截止时间，用于 git-stats-*，默认 $(UNTIL)"
	@echo "  SUBMODULE_URL      子模块远程地址，用于 submodule-add"
	@echo "  SUBMODULE_PATH     子模块本地路径，用于 submodule-add / submodule-remove"
	@echo "  DEPLOY_BIN         要部署的二进制路径，默认 $(DEPLOY_BIN)"
	@echo "                     示例：make deploy-release DEPLOY_BIN=bin/examples/pointslistmodel/example_pointslistmodel"
	@echo "  NPROC              并行编译核数，默认 $(NPROC)"
	@echo ""
	@echo "Init:"
	@echo "  init               项目初始化：拉取子模块 + 安装 proto 工具 + 生成 pb/"
	@echo ""
	@echo "Submodule:"
	@echo "  submodule-update   拉取/更新所有 git 子模块"
	@echo "  submodule-add      添加 git 子模块（需传 SUBMODULE_URL 和 SUBMODULE_PATH）"
	@echo "  submodule-remove   移除 git 子模块（需传 SUBMODULE_PATH）"
	@echo ""
	@echo "3rdparty:"
	@echo "  qtmqtt-build       编译 qtmqtt（shadow build 到 3rdparty/qtmqtt/build/）"
	@echo "  qtmqtt-install     编译并安装 qtmqtt 到 3rdparty/qtmqtt/intall/"
	@echo "  qtmqtt-clean       清理 qtmqtt 编译产物和安装目录"
	@echo ""
	@echo "Build:"
	@echo "  build-debug        编译主项目（debug 模式，输出到 build/debug/）"
	@echo "  build-release      编译主项目（release 模式，输出到 build/release/）"
	@echo "  build-clean        清理主项目编译产物（删除 build/）"
	@echo ""
	@echo "Deploy:"
	@echo "  deploy-debug       打包 debug 构建（二进制 + Qt 库 + 第三方库 + 插件）"
	@echo "  deploy-release     打包 release 构建（二进制 + Qt 库 + 第三方库 + 插件）"
	@echo "  deploy-clean       清理打包产物（删除 deploy/）"
	@echo ""
	@echo "Doc:"
	@echo "  doc-new            在 doc/ 下创建 VERSION 版本目录结构（issues/ fix/ plan/）"
	@echo ""
	@echo "Git:"
	@echo "  git-stats-commits  统计 AUTHOR 在 SINCE~UNTIL 内的提交数"
	@echo "  git-stats-c++      统计 AUTHOR 在 SINCE~UNTIL 内的代码变更行数"
	@echo "  git-stats-md       统计 doc 子仓库中 AUTHOR 在 SINCE~UNTIL 内的文档变更行数"
	@echo ""
	@echo "  help               显示此帮助信息"