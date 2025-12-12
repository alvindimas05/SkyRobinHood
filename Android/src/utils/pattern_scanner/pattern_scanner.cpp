#include "pattern_scanner.hpp"

uintptr_t PatternScanner::Find(const uint8_t *pattern, const char *mask, uintptr_t start, size_t search_size)
{
    std::vector<MemoryRegion> regions = GetExecutableRegions();

    for (const auto &region : regions)
    {
        if (start != 0 && region.base + region.size < start)
        {
            continue;
        }

        uintptr_t search_start = (start > region.base) ? start : region.base;
        size_t offset = search_start - region.base;
        size_t size = region.size - offset;

        if (search_size > 0 && size > search_size)
        {
            size = search_size;
        }

        uintptr_t result = SearchInRegion(region.data + offset, size, pattern, mask);
        if (result != 0)
        {
            return search_start + result;
        }

        if (search_size > 0)
        {
            search_size -= size;
            if (search_size == 0)
                break;
        }
    }

    return 0;
}

uintptr_t PatternScanner::Find(const std::vector<uint8_t> &pattern, const std::string &mask)
{
    return Find(pattern.data(), mask.c_str());
}

uintptr_t PatternScanner::SearchInRegion(const uint8_t *data, size_t size, const uint8_t *pattern, const char *mask)
{
    size_t pattern_len = strlen(mask);

    if (pattern_len == 0 || size < pattern_len)
    {
        return 0;
    }

    for (size_t i = 0; i <= size - pattern_len; i++)
    {
        bool found = true;
        for (size_t j = 0; j < pattern_len; j++)
        {
            if (mask[j] != '?' && data[i + j] != pattern[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return i;
        }
    }

    return 0;
}

std::vector<PatternScanner::MemoryRegion> PatternScanner::GetExecutableRegions()
{
    std::vector<PatternScanner::MemoryRegion> regions;

#ifdef _WIN32
    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t address = 0;

    while (VirtualQuery((LPCVOID)address, &mbi, sizeof(mbi)))
    {
        if (mbi.State == MEM_COMMIT &&
            (mbi.Protect & PAGE_EXECUTE_READ ||
             mbi.Protect & PAGE_EXECUTE_READWRITE ||
             mbi.Protect & PAGE_EXECUTE_WRITECOPY))
        {

            MemoryRegion region;
            region.base = (uintptr_t)mbi.BaseAddress;
            region.size = mbi.RegionSize;
            region.data = (uint8_t *)mbi.BaseAddress;
            regions.push_back(region);
        }

        address = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
    }
#elif defined(__ANDROID__)
    dl_iterate_phdr([](struct dl_phdr_info *info, size_t size, void *data) -> int
                    {
        auto* regions = static_cast<std::vector<MemoryRegion>*>(data);
        
        // Only scan libBootloader.so
        if (!info->dlpi_name || !strstr(info->dlpi_name, "libBootloader.so")) {
            return 0;
        }
        
        for (int i = 0; i < info->dlpi_phnum; i++) {
            const ElfW(Phdr)* phdr = &info->dlpi_phdr[i];
            
            if (phdr->p_type == PT_LOAD && (phdr->p_flags & PF_X)) {
                MemoryRegion region;
                region.base = info->dlpi_addr + phdr->p_vaddr;
                region.size = phdr->p_memsz;
                region.data = (uint8_t*)region.base;
                regions->push_back(region);
            }
        }
        
        return 0; }, &regions);
#else
    // Linux/Unix implementation using /proc/self/maps
    std::ifstream maps("/proc/self/maps");
    std::string line;

    while (std::getline(maps, line))
    {
        std::istringstream iss(line);
        std::string addr_range, perms;
        iss >> addr_range >> perms;

        // Check if region is executable and readable
        if (perms.find('r') != std::string::npos &&
            perms.find('x') != std::string::npos)
        {

            size_t dash_pos = addr_range.find('-');
            uintptr_t start = std::stoull(addr_range.substr(0, dash_pos), nullptr, 16);
            uintptr_t end = std::stoull(addr_range.substr(dash_pos + 1), nullptr, 16);

            MemoryRegion region;
            region.base = start;
            region.size = end - start;
            region.data = (uint8_t *)start;
            regions.push_back(region);
        }
    }
#endif

    return regions;
}