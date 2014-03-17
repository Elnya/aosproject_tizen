#ifndef _LINUX_VIRTIO_IDS_H
#define _LINUX_VIRTIO_IDS_H
/*
 * Virtio IDs
 *
 * This header is BSD licensed so anyone can use the definitions to implement
 * compatible drivers/servers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of IBM nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL IBM OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE. */

#define VIRTIO_ID_NET		1 /* virtio net */
#define VIRTIO_ID_BLOCK		2 /* virtio block */
#define VIRTIO_ID_CONSOLE	3 /* virtio console */
#define VIRTIO_ID_RNG		4 /* virtio ring */
#define VIRTIO_ID_BALLOON	5 /* virtio balloon */
#define VIRTIO_ID_RPMSG		7 /* virtio remote processor messaging */
#define VIRTIO_ID_SCSI		8 /* virtio scsi */
#define VIRTIO_ID_9P		9 /* 9p virtio console */

#ifdef CONFIG_MARU
/* Maru devices */
#define VIRTIO_ID_TOUCHSCREEN	31 /* virtio touchscreen */
#define VIRTIO_ID_KEYBOARD		32 /* virtio keyboard */
#define VIRTIO_ID_ESM			33 /* virtio Emulator Status Monitor */
#define VIRTIO_ID_HWKEY			34 /* virtio hwkey */
#define VIRTIO_ID_EVDI			35 /* virtio emulator virtual device interface */
#define VIRTIO_ID_GL			36 /* virtio glmem */
#define VIRTIO_ID_SENSOR		37 /* virtio sensor */
#define VIRTIO_ID_NFC			38 /* virtio nfc */
#define VIRTIO_ID_JACK			39 /* virtio jack */
#define VIRTIO_ID_POWER			40 /* virtio power supply */
#endif

#endif /* _LINUX_VIRTIO_IDS_H */