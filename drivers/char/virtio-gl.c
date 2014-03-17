/*
 * Copyright (C) 2010 Intel Corporation
 *
 * Author: Ian Molton <ian.molton@collabora.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/virtio.h>
#include <linux/virtio_ids.h>
#include <linux/virtio_config.h>

//#define VIRTIO_ID_GL 36 /* use "include/linux/virtio_ids.h" file instead */
#define DEVICE_NAME "glmem"

/* Define to use debugging checksums on transfers */
#undef DEBUG_GLIO

struct virtio_gl_data {
	char *buffer;
	int pages;
	unsigned int pid;
};

struct virtio_gl_header {
	int pid;
	int buf_size;
	int r_buf_size;
#ifdef DEBUG_GLIO
	int sum;
#endif
	char buffer;
} __packed;

#define to_virtio_gl_data(a)   ((struct virtio_gl_data *)(a)->private_data)

#ifdef DEBUG_GLIO
#define SIZE_OUT_HEADER (sizeof(int)*4)
#define SIZE_IN_HEADER (sizeof(int)*2)
#else
#define SIZE_OUT_HEADER (sizeof(int)*3)
#define SIZE_IN_HEADER sizeof(int)
#endif

static struct virtqueue *vq;

static DEFINE_MUTEX(gl_mutex);

/* This is videobuf_vmalloc_to_sg() from videobuf-dma-sg.c with
 * some modifications
 */
static struct scatterlist *vmalloc_to_sg(struct scatterlist *sg_list,
				unsigned char *virt, unsigned int pages)
{
	struct page *pg;

	/* unaligned */
	BUG_ON((ulong)virt & ~PAGE_MASK);

	/* Fill with elements for the data */
	while (pages) {
		pg = vmalloc_to_page(virt);
		if (!pg)
			goto err;

		sg_set_page(sg_list, pg, PAGE_SIZE, 0);
		virt += PAGE_SIZE;
		sg_list++;
		pages--;
	}

	return sg_list;

err:
	kfree(sg_list);
	return NULL;
}

static int put_data(struct virtio_gl_data *gldata)
{
	struct scatterlist *sg, *sg_list;
	unsigned int count, ret, o_page, i_page, sg_entries;
	struct virtio_gl_header *header =
				(struct virtio_gl_header *)gldata->buffer;

	mutex_lock(&gl_mutex);

	ret = header->buf_size;

	o_page = (header->buf_size + PAGE_SIZE-1) >> PAGE_SHIFT;
	i_page = (header->r_buf_size + PAGE_SIZE-1) >> PAGE_SHIFT;

	header->pid = gldata->pid;

	if ((o_page && i_page) &&
		(o_page > gldata->pages || i_page > gldata->pages)) {
		i_page = 0;
	}

	if (o_page > gldata->pages)
		o_page = gldata->pages;

	if (i_page > gldata->pages)
		i_page = gldata->pages;

	if (!o_page)
		o_page = 1;

	sg_entries = o_page + i_page;

	sg_list = kcalloc(sg_entries, sizeof(struct scatterlist), GFP_KERNEL);

	if (!sg_list) {
		ret = -EIO;
		goto out;
	}

	sg_init_table(sg_list, sg_entries);

	sg = vmalloc_to_sg(sg_list, gldata->buffer, o_page);
	sg = vmalloc_to_sg(sg, gldata->buffer, i_page);

	if (!sg) {
		ret = -EIO;
		goto out_free;
	}

#if 0
	/* Transfer data */
	if (vq->vq_ops->add_buf(vq, sg_list, o_page, i_page, (void *)1) >= 0) {
		vq->vq_ops->kick(vq);
		/* Chill out until it's done with the buffer. */
		while (!vq->vq_ops->get_buf(vq, &count))
			cpu_relax();
	}
#else
	/* The fifth parameter of virtqueue_add_buf : FIX ME */
	/* Transfer data */
	if (virtqueue_add_buf(vq, sg_list, o_page, i_page, (void *)1, GFP_ATOMIC) >= 0) {
		virtqueue_kick(vq);
		/* Chill out until it's done with the buffer. */
		while (!virtqueue_get_buf(vq, &count))
			cpu_relax();
	}
#endif

out_free:
	kfree(sg_list);
out:
	mutex_unlock(&gl_mutex);
	return ret;
}

static void free_buffer(struct virtio_gl_data *gldata)
{
	if (gldata->buffer) {
		vfree(gldata->buffer);
		gldata->buffer = NULL;
	}
}

static int glmem_open(struct inode *inode, struct file *file)
{
	struct virtio_gl_data *gldata = kzalloc(sizeof(struct virtio_gl_data),
						GFP_KERNEL);

	if (!gldata)
		return -ENXIO;

	gldata->pid = pid_nr(task_pid(current));

	file->private_data = gldata;

	return 0;
}

static int glmem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct virtio_gl_data *gldata = to_virtio_gl_data(filp);
	int pages = (vma->vm_end - vma->vm_start) / PAGE_SIZE;

	/* Set a reasonable limit */
	if (pages > 16)
		return -ENOMEM;

	/* for now, just allow one buffer to be mmap()ed. */
	if (gldata->buffer)
		return -EIO;

	gldata->buffer = vmalloc_user(pages*PAGE_SIZE);

	if (!gldata->buffer)
		return -ENOMEM;

	gldata->pages = pages;

	if (remap_vmalloc_range(vma, gldata->buffer, 0) < 0) {
		vfree(gldata->buffer);
		return -EIO;
	}

	vma->vm_flags |= VM_DONTEXPAND;

	return 0;
}

static int glmem_fsync(struct file *filp, loff_t start, loff_t end, int datasync)
{
	struct virtio_gl_data *gldata = to_virtio_gl_data(filp);

	put_data(gldata);

	return 0;
}

static int glmem_release(struct inode *inode, struct file *file)
{
	struct virtio_gl_data *gldata = to_virtio_gl_data(file);

	if (gldata && gldata->buffer) {
		struct virtio_gl_header *header =
				(struct virtio_gl_header *)gldata->buffer;

		/* Make sure the host hears about the process ending / dying */
		header->pid = gldata->pid;
		header->buf_size = SIZE_OUT_HEADER + 2;
		header->r_buf_size = SIZE_IN_HEADER;
		*(short *)(&header->buffer) = -1;

		put_data(gldata);
		free_buffer(gldata);
	}

	kfree(gldata);

	return 0;
}

static const struct file_operations glmem_fops = {
	.owner		= THIS_MODULE,
	.open		= glmem_open,
	.mmap		= glmem_mmap,
	.fsync		= glmem_fsync,
	.release	= glmem_release,
};

static struct miscdevice glmem_dev = {
	MISC_DYNAMIC_MINOR,
	DEVICE_NAME,
	&glmem_fops
};

static int glmem_probe(struct virtio_device *vdev)
{
	int ret;

	/* We expect a single virtqueue. */
	vq = virtio_find_single_vq(vdev, NULL, "output");
	if (IS_ERR(vq))
		return PTR_ERR(vq);

	ret = misc_register(&glmem_dev);
	if (ret) {
		printk(KERN_ERR "glmem: cannot register glmem_dev as misc");
		return -ENODEV;
	}

	return 0;
}

static void __devexit glmem_remove(struct virtio_device *vdev)
{
	vdev->config->reset(vdev);
	misc_deregister(&glmem_dev);
	vdev->config->del_vqs(vdev);
}

static struct virtio_device_id id_table[] = {
	{ VIRTIO_ID_GL, VIRTIO_DEV_ANY_ID },
	{ 0 },
};

static struct virtio_driver virtio_gl_driver = {
	.driver	= {
		.name =  KBUILD_MODNAME,
		.owner = THIS_MODULE,
	},
	.id_table =     id_table,
	.probe =        glmem_probe,
	.remove =       __devexit_p(glmem_remove),
};

static int __init glmem_init(void)
{
	return register_virtio_driver(&virtio_gl_driver);
}

static void __exit glmem_exit(void)
{
	unregister_virtio_driver(&virtio_gl_driver);
}

module_init(glmem_init);
module_exit(glmem_exit);

MODULE_DEVICE_TABLE(virtio, id_table);
MODULE_DESCRIPTION("Virtio gl passthrough driver");
MODULE_LICENSE("GPL v2");

