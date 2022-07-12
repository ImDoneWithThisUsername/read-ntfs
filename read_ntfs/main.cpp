#include "function.h"
#include "Struct_MFT.h"
#include "MFT.h"

int main(int argc, char ** argv)
{
    LPCWSTR path = L"\\\\.\\F:";


    BYTE sector[512];
    ReadSector(path,0, sector);

    //read sector size
    int sector_byte_size = read_little_eldian_to_dec(sector, hex_to_dec("0x0B"), 2);
    //read sector per cluster
    int sector_per_cluster = read_little_eldian_to_dec(sector, hex_to_dec("0x0D"), 1);

    // byte offset of first mft entry
    string mft_cluster_offset = read_byte(sector, hex_to_dec(START_OF_MFT_CLUSTER_OFFSET), 8);
    

    //convert from big eldian to little eldian
    mft_cluster_offset = to_big_eldian(mft_cluster_offset);

    // byte offset of first mft entry
    unsigned long long int mft_byte_offset = hex_to_dec(mft_cluster_offset) * (sector_byte_size * sector_per_cluster);
    cout << "NTFS partition bootsector" << endl;
    cout << "Sector size: " << sector_byte_size << " bytes" << endl;
    cout << "Sector per cluster: " << sector_per_cluster << " sectors" << endl;
    cout << "First MFT entry cluster: " << mft_cluster_offset << endl;
    cout << "First MFT entry sector: " << hex_to_dec(mft_cluster_offset) * sector_per_cluster << endl;
    cout << "First MFT entry byte: " << mft_byte_offset << endl;
    cout << endl;
    cout << "Press Enter to continue";
    cin.get();
    //read mft entry
    vector<MFT> list_MFT;
    for (int i = 0; i < 50; i++)
    {
        BYTE sector1[512], sector2[512];
        ReadSector(path, mft_byte_offset, sector1);
        if (read_char(sector1, 0, 4) == "FILE")
        {

            ReadSector(path, mft_byte_offset + 512, sector2);
            MFT mft_entry(sector1, sector2, mft_byte_offset);
            //if(mft_entry.isMFT && mft_entry.isArchieve)
            if (mft_entry.isMFT)
                list_MFT.push_back(mft_entry);
        }

        mft_byte_offset += 1024;
    }
    cout << "--------------" << endl;
    cout << "MFT entry" << endl;
    for (int i = 0; i < list_MFT.size(); i++)
    {
        cout << i + 1 << "/" << list_MFT[i].file_name.name_of_file << endl;
    }
    while (1) {
        int choice;
        cout << "\nInput file order to read (0 to exit): ";
        cin >> choice;
        if (choice == 0)
        {
            break;
        }
        if (choice - 1 < 0 || choice - 1 >= list_MFT.size())
        {
            cout << "Out of range" << endl;
            cout << "There is only " << list_MFT.size() << " mft entry" << endl;
            continue;
        }
        cout << "Sector index:\t\t\t " << list_MFT[choice - 1].offset / sector_byte_size << endl;
        list_MFT[choice - 1].out_put();
    }
    return 0;
}
