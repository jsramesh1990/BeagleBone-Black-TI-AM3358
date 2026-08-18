SUMMARY = "Box Storage System"
DESCRIPTION = "Date indexed static and dynamic storage application"
LICENSE = "MIT"

SRC_URI = " \
    file://box-storage.service \
    file://boxes.conf \
    file://logging.conf \
"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE:${PN} = "box-storage.service"
SYSTEMD_AUTO_ENABLE = "enable"

do_compile() {
    ${CXX} ${CXXFLAGS} \
        -std=c++17 \
        ${WORKDIR}/../../app/src/Date.cpp \
        ${WORKDIR}/../../app/src/Entry.cpp \
        ${WORKDIR}/../../app/src/StaticBox.cpp \
        ${WORKDIR}/../../app/src/DynamicBox.cpp \
        ${WORKDIR}/../../app/src/BoxManager.cpp \
        ${WORKDIR}/../../app/src/main.cpp \
        -I${WORKDIR}/../../app/include \
        -o boxes
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 boxes ${D}${bindir}/boxes

    install -d ${D}${sysconfdir}/box-storage

    install -m 0644 \
        ${WORKDIR}/boxes.conf \
        ${D}${sysconfdir}/box-storage/boxes.conf

    install -m 0644 \
        ${WORKDIR}/logging.conf \
        ${D}${sysconfdir}/box-storage/logging.conf

    install -d ${D}${systemd_system_unitdir}

    install -m 0644 \
        ${WORKDIR}/box-storage.service \
        ${D}${systemd_system_unitdir}/box-storage.service
}

FILES:${PN} += " \
    ${bindir}/boxes \
    ${sysconfdir}/box-storage/ \
    ${systemd_system_unitdir}/box-storage.service \
"
