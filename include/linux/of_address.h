/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __OF_ADDRESS_H
#define __OF_ADDRESS_H
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/of.h>
#include <linux/io.h>

struct of_pci_range_parser {
	struct device_node *node;
	const __be32 *range;
	const __be32 *end;
	int np;
	int pna;
	bool dma;
};

struct of_pci_range {
	u32 pci_space;
	u64 pci_addr;
	u64 cpu_addr;
	u64 size;
	u32 flags;
};

#define for_each_of_pci_range(parser, range) \
	for (; of_pci_range_parser_one(parser, range);)

/* Translate a DMA address from device space to CPU space */
extern u64 of_translate_dma_address(struct device_node *dev,
				    const __be32 *in_addr);

#ifdef CONFIG_OF_ADDRESS
extern u64 of_translate_address(struct device_node *np, const __be32 *addr);
extern int of_address_to_resource(struct device_node *dev, int index,
				  struct resource *r);
extern struct device_node *of_find_matching_node_by_address(
					struct device_node *from,
					const struct of_device_id *matches,
					u64 base_address);
extern void __iomem *of_iomap(struct device_node *device, int index);
extern void __iomem *of_iomap_by_name(struct device_node *device,
					const char *name);
void __iomem *of_io_request_and_map(struct device_node *device,
				    int index, const char *name);

/* Extract an address from a device, returns the region size and
 * the address space flags too. The PCI version uses a BAR number
 * instead of an absolute index
 */
extern const __be32 *of_get_address(struct device_node *dev, int index,
			   u64 *size, unsigned int *flags);
extern const __be32 *of_get_address_by_name(struct device_node *dev,
			   const char *name, u64 *size, unsigned int *flags);

extern int of_pci_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node);
extern int of_pci_dma_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node);
extern struct of_pci_range *of_pci_range_parser_one(
					struct of_pci_range_parser *parser,
					struct of_pci_range *range);
extern bool of_dma_is_coherent(struct device_node *np);

#if defined(CONFIG_DMA_COHERENT_HINT_CACHED)
extern bool of_dma_is_coherent_hint_cached(struct device_node *np);
#else /* CONFIG_DMA_COHERENT_HINT_CACHED */
static inline bool of_dma_is_coherent_hint_cached(struct device_node *np)
{
	return false;
}
#endif /* CONFIG_DMA_COHERENT_HINT_CACHED */

#else /* CONFIG_OF_ADDRESS */
static inline void __iomem *of_io_request_and_map(struct device_node *device,
						  int index, const char *name)
{
	return IOMEM_ERR_PTR(-EINVAL);
}

static inline u64 of_translate_address(struct device_node *np,
				       const __be32 *addr)
{
	return OF_BAD_ADDR;
}

static inline struct device_node *of_find_matching_node_by_address(
					struct device_node *from,
					const struct of_device_id *matches,
					u64 base_address)
{
	return NULL;
}

static inline const __be32 *of_get_address(struct device_node *dev, int index,
					u64 *size, unsigned int *flags)
{
	return NULL;
}

static inline int of_pci_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node)
{
	return -ENOSYS;
}

static inline int of_pci_dma_range_parser_init(struct of_pci_range_parser *parser,
			struct device_node *node)
{
	return -ENOSYS;
}

static inline struct of_pci_range *of_pci_range_parser_one(
					struct of_pci_range_parser *parser,
					struct of_pci_range *range)
{
	return NULL;
}

static inline bool of_dma_is_coherent(struct device_node *np)
{
	return false;
}

static inline bool of_dma_is_coherent_hint_cached(struct device_node *np)
{
	return false;
}

#endif /* CONFIG_OF_ADDRESS */

#ifdef CONFIG_OF
extern int of_address_to_resource(struct device_node *dev, int index,
				  struct resource *r);
void __iomem *of_iomap(struct device_node *node, int index);
#else
static inline int of_address_to_resource(struct device_node *dev, int index,
					 struct resource *r)
{
	return -EINVAL;
}

static inline void __iomem *of_iomap(struct device_node *device, int index)
{
	return NULL;
}
#endif

#if defined(CONFIG_OF_ADDRESS) && defined(CONFIG_PCI)
extern const __be32 *of_get_pci_address(struct device_node *dev, int bar_no,
			       u64 *size, unsigned int *flags);
extern int of_pci_address_to_resource(struct device_node *dev, int bar,
				      struct resource *r);
extern int of_pci_range_to_resource(struct of_pci_range *range,
				    struct device_node *np,
				    struct resource *res);
#else /* CONFIG_OF_ADDRESS && CONFIG_PCI */
static inline int of_pci_address_to_resource(struct device_node *dev, int bar,
				             struct resource *r)
{
	return -ENOSYS;
}

static inline const __be32 *of_get_pci_address(struct device_node *dev,
		int bar_no, u64 *size, unsigned int *flags)
{
	return NULL;
}
static inline int of_pci_range_to_resource(struct of_pci_range *range,
					   struct device_node *np,
					   struct resource *res)
{
	return -ENOSYS;
}
#endif /* CONFIG_OF_ADDRESS && CONFIG_PCI */

#endif /* __OF_ADDRESS_H */

