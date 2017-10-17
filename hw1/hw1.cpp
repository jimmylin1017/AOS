#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <ctime>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

/*
1.  Reference string: 1~350 
2.  Number of memory references: At least 70,000 times 
3.  Number of frames in the physical memory: 10, 20, 30, 40, 50, 60, 70 
4.  Three test reference strings: 
(1) Random: Arbitrarily pick one number for each reference. 
(2) Locality:  Simulate  function  calls.  Each  function  call  may  refer  a  subset  of  1/6~1/4 
string (the length of string can be random).   
(3) Your  own  reference  string.  However,  you  should  discuss  why  you  choose  such  a 
reference string in the report. 
5.  You can use both reference and dirty bits. 
*/


#define REFER_STR 350 // reference string
#define MEM_REFER 70000 // number of memory references

#define LOCAL_RANGE 70 // for reference string range, ex: REFER_STR / LOCAL_RANGE
#define LOCAL_LEN 5000 // for number of memory references, ex: MEM_REFER / LOCAL_LEN


/*
#define REFER_STR 25 // reference string
#define MEM_REFER 50 // number of memory references

#define LOCAL_RANGE 5 // for reference string range, ex: REFER_STR / LOCAL_RANGE
#define LOCAL_LEN 10 // for number of memory references, ex: MEM_REFER / LOCAL_LEN
*/

struct MemoryFrame
{
    int data;
    int reference;
    int dirty;
};

int memory_reference[MEM_REFER];

/* Random: Arbitrarily pick one number for each reference. */
void create_random_reference_string()
{
    memset(memory_reference, 0, MEM_REFER);

    // reference string: 1~350
    for(int i=0; i<MEM_REFER; i++)
        memory_reference[i] = rand() % REFER_STR + 1;
}

/* Locality: Simulate function calls. Each function call may refer a subset of 1/6~1/4 string (the length of string can be random). */
void create_locality_reference_string()
{
    int base_num = 1;

    memset(memory_reference, 0, MEM_REFER);

    // reference string: 1~350
    for(int i=0; i<MEM_REFER; i++)
    {
        // random: 0~69 + base_num, base_num = 1, 71, 141, 211, 281
        memory_reference[i] = rand() % LOCAL_RANGE + base_num;

        if((i + 1) % LOCAL_LEN == 0)
        {
            base_num += LOCAL_RANGE;
        }
    }
}

void create_own_reference_string()
{
    
}

/*
(1) FIFO algorithm
(2) Optimal algorithm
(3) Enhanced second-chance algorithm
*/

/* FIFO algorithm, interrupt count with write back and page fault */
void fifo_algorithm(int memory_size)
{
    cout<<"memory_size: "<<memory_size<<endl;

    vector<MemoryFrame> memory_frames;

    int interrupt_counter = 0;
    int page_fault = 1;
    
    for(int i=0; i<MEM_REFER; i++)
    {
        page_fault = 1;

        // find data in memory frames
        for(int j=0; j<memory_frames.size(); j++)
        {
            if(memory_frames[j].data == memory_reference[i])
            {
                page_fault = 0;
                break;
            }
        }

        if(page_fault == 0)
        {
            //cout<<"Find: "<<memory_reference[i]<<'\n';
            continue;
        }

        // check if memory frames is full
        if(memory_frames.size() == memory_size)
        {
            if(memory_frames[0].dirty)
            {
                //cout<<"write back: "<<memory_frames[0].data<<endl;
                interrupt_counter++;
            }

            // memory frames full then erase first data
            memory_frames.erase(memory_frames.begin());
        }

        // add new data to memory frames
        MemoryFrame temp;
        temp.data = memory_reference[i];
        temp.reference = 0;
        temp.dirty = random() / 7 % 2;

        memory_frames.push_back(temp);
        interrupt_counter++;

        // print out current memory frames status
        /*for(int j=0; j<memory_frames.size(); j++)
        {
            cout<<" "<<memory_frames[j].data;
        }

        cout<<"\n========================\n";*/
    }

    cout<<"interrupt_counter: "<<interrupt_counter<<endl;
}

void optimal_algorithm(int memory_size)
{

}

void enhanced_second_chance_algorithm(int memory_size)
{

}

void own_algorithm(int memory_size)
{

}

int main()
{
    // initialize random seed
    srand (time(NULL));

    // running random reference string
    create_random_reference_string();
    cout<<"=====create_random_reference_string====="<<endl;
    for(int i=10; i<=70; i+=10)
    {
        fifo_algorithm(i);
    }
    
    // running locality reference string
    create_locality_reference_string();
    cout<<"\n=====create_locality_reference_string====="<<endl;
    for(int i=10; i<=70; i+=10)
    {
        fifo_algorithm(i);
    }

    // print out reference string
    /*for(int i=0; i<MEM_REFER; i++)
        cout<<memory_reference[i]<<" ";
    cout<<'\n';*/

    return 0;
}