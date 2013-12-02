#ifndef __BITVECTOR_H__
#define __BITVECTOR_H__

#include <IceUtil/IceUtil.h>
#include <sys/types.h>
#include "UtilCxx/src/LogWrapper.h"

/********************************************************************
  ��������: λ������
  ��һ��bit����ʾһ��Ԫ�ص�״̬��ֻ�ܱ�ʾ����״̬�� 0��1�� 
  ������һ��char���Ա�ʾ8��Ԫ�ص�״̬����ʡ�ڴ�ռ䡣
  ��Ҫ����ӿ�: 
  Get()����ȡ��n��Ԫ�ص�״̬
  Set()�����õ�n��Ԫ�ص�״̬Ϊ1
 *********************************************************************/    
#define USE_LOCK //ʹ�û�����
class CBitVector
{
 private:
   /** ��װ�ص�Ԫ�ص������Ŀ */
   int m_dwSize;

   /** ʵ��װ�ص�Ԫ�ص���Ŀ*/
   int m_dwCount;

   /** װ��λ�������ݵ��ֽ����� */
   char* m_pBits;
#ifdef USE_LOCK
   IceUtil::RWRecMutex bits_lock_;
#endif 

 public:
   /********************************************************************
     ��������: ���캯��. ����һ���ܹ���ʾdwSize��Ԫ�ص�λ����
     �������: dwSize λ������ʾԪ�ص���Ŀ                  
     ���� :   �� 
    *********************************************************************/
   CBitVector(int dwSize)
   {
     m_dwCount = -1;
     m_dwSize = (dwSize <= 0) ? 0 : dwSize;

     //ʵ����Ҫ������ֽ���Ŀ��dwSize��1/8����Ϊһ���ֽڿ��Ա�ʾ8��״̬��
     int dwLen = (m_dwSize >> 3) + 1;
     m_pBits = new char[dwLen];
     for (int i = 0;i < dwLen;i++)
     {
       m_pBits[i] = 0;
     }
#ifdef USE_LOCK
     MCE_DEBUG("[TEMPLE] Use mutex lock in bitmap");
     std::cout << "Use Mutex lock in bitmap" << std::endl;
#else
     MCE_DEBUG("[TEMPLE] DO Not Use mutex lock in bitmap");
     std::cout << "Use NO Mutex lock in bitmap" << std::endl;
#endif
   }

   /** �����������ͷ�ռ�õĿռ䡣 */
   ~CBitVector()
   {
     if (m_pBits)
     {
       delete m_pBits;
       m_pBits = NULL;
     }
   }

   /********************************************************************
     ��������: ���õ�n��Ԫ�ص�״̬Ϊ1
     �������: n  ��n��Ԫ��                  
     ���� :   �� 
    *********************************************************************/
   inline void Set(int n)
   {
     if (n >= m_dwSize || n < 0) {
       std::cout << "Fatal! n is " << n << ", m_dwSize is " << m_dwSize << std::endl;
       return;
     }
     //MCE_DEBUG("[TEMPLE] Step in BitMap::Set, uid is " << (int)n);
#ifdef USE_LOCK
     IceUtil::RWRecMutex::WLock lock(bits_lock_);
#endif
     m_pBits[n >> 3] |= 1 << (n & 7);
     m_dwCount = -1;
   }

   /********************************************************************
     ��������: ��ȡ��n��Ԫ�ص�״̬
     �������: n  ��n��Ԫ��                  
     ���� :   ��״̬Ϊ1���򷵻�TRUE;��״̬Ϊ0���򷵻�FALSE 
    *********************************************************************/
   inline bool Get(int n)
   {
     if (n >= m_dwSize || n < 0) {
       return 0;
     }
     //MCE_DEBUG("[TEMPLE] Step in BitMap::Get, uid is " << (int)n );
#ifdef USE_LOCK
     IceUtil::RWRecMutex::RLock lock(bits_lock_);
#endif
     return (m_pBits[n >> 3] & (1 << (n & 7))) != 0;
   }

   /********************************************************************
     ��������: ��ȡ��start_pos��ʼ��64��Ԫ�ص�״̬
     �������: start_pos  ��ʼԪ��                  
     �������: 
     ���� :  ������λ����ͨ�������Ϊ64�� ����ʼλ��Ϊ���63��ʱ������
    *********************************************************************/
   inline int GetsValidInNext64Bit(int start_pos, std::vector<int>& valid_ids)
   {
     int return_value = 0;
     if (start_pos < 0 || start_pos > m_dwSize) {
       return return_value;
     }
#ifdef USE_LOCK
     IceUtil::RWRecMutex::RLock lock(bits_lock_);
#endif
     __int64_t temp_int;
     //ע��"=="���ȼ���"^"��!!!
     //���64λ(8���ֽ�), ��ֹԽ��
     if (0 == ((start_pos >> 6) ^ (m_dwSize >> 6))) {
       temp_int = (*(__int64_t*)&m_pBits[start_pos >> 6]); 
     } else {
       temp_int = (*(__int64_t*)(&m_pBits[start_pos >> 3])); 
     }

     __int64_t offset = (__int64_t)start_pos & 0x3F;

     for(__int64_t i=offset; i<64; ++i) {
       __int64_t mask = (__int64_t)1 << i;
       if(temp_int & mask) {
         valid_ids.push_back(start_pos + i);
       }
     }

     return (64-offset);
   }


   /********************************************************************
     ��������: �����n��Ԫ�ص�״̬Ϊ0
     �������: n  ��n��Ԫ��                  
     ���� :   �� 
    *********************************************************************/
   inline void Clear(int n)
   {
     if (n >= m_dwSize || n < 0) {
       return ;
     }
#ifdef USE_LOCK
     IceUtil::RWRecMutex::WLock lock(bits_lock_);
#endif
     m_pBits[n >> 3] &= ~(1 << (n & 7));
     m_dwCount = -1;
   }

   inline void Clear()
   {
#ifdef USE_LOCK
     IceUtil::RWRecMutex::WLock lock(bits_lock_);
#endif
     int dwLen = (m_dwSize >> 3) + 1;            
     for (int i = 0;i < dwLen;i++)
     {
       m_pBits[i] = 0;
     }
   }

   /********************************************************************
     ��������: ��ȡ��װ�ص�Ԫ�ص������Ŀ
     �������: ��                  
     ���� :   ��װ�ص�Ԫ�ص������Ŀ
    *********************************************************************/
   inline int Size()
   {
     return m_dwSize;
   }

   /********************************************************************
     ��������: ��ȡʵ��װ�ص�Ԫ�ص���Ŀ
     �������: ��                  
     ���� :   ʵ��װ�ص�Ԫ�ص���Ŀ
    *********************************************************************/
   inline int Count()
   {
     MCE_DEBUG("[TEMPLE] Step in BitMap::Count");
#ifdef USE_LOCK
     IceUtil::RWRecMutex::WLock lock(bits_lock_);
#endif
     if (-1 == m_dwCount) 
     {
       char BYTE_COUNTS[] = {	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
         4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8	};

       int c = 0;
       int dwEnd = (m_dwSize >> 3) + 1;
       for (int i = 0; i < dwEnd; i++)
       {
         c += BYTE_COUNTS[m_pBits[i] & 0xFF];	 
       }
       m_dwCount = c;
     }

     return m_dwCount;
   }

   //���浽�ļ�
   int Save(const char* szpFileName)
   {
#ifdef USE_LOCK
     IceUtil::RWRecMutex::RLock lock(bits_lock_);
#endif
     FILE* fp= fopen(szpFileName,"wb");
     if(!fp)
     {				
       return -1;
     }

     //��ȡbitmap����			
     int dwLen = (m_dwSize >> 3)+1 ;
     if (fwrite(m_pBits,sizeof(char),dwLen,fp) != (size_t)dwLen)
     {
       fclose(fp);
       return -2;
     }
     fclose(fp);
     return 0;		
   }

   //�����ļ����ڴ�
   int Load(const char* szpFileName)
   {
#ifdef USE_LOCK
     IceUtil::RWRecMutex::WLock lock(bits_lock_);
#endif
     FILE* fp= fopen(szpFileName,"rb");
     if(!fp)
     {				
       return -1;
     }

     //���ļ���λ��ָ���Ƶ��ļ�β
     fseek(fp,0,SEEK_END);
     //��ȡ�ļ�����
     int dwSize=ftell(fp);
     //���ļ���������m_pBits
     fseek(fp,0,SEEK_SET);
     if (fread(m_pBits,sizeof(char),dwSize,fp) != (size_t)dwSize)
     {
       fclose(fp);
       return -2;
     }

     fclose(fp);
     return 0;
   }
};
#endif
