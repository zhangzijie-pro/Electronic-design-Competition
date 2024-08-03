import os
import random
# 递归遍历文件夹下所有txt文件
def get_files(path):
    files = []
    for root, dirs, file_names in os.walk(path):
        for file_name in file_names:
            if file_name.endswith('.txt') and 'proc' not in file_name:
                files.append(os.path.join(root, file_name))

    return files

if __name__ == '__main__':
    folder = 'music'
    files = get_files(folder)
    print(folder, files)

    for file in files:
        with open(file, 'r', encoding='utf-8') as f:
            print("++++", file, "++++")
            lines = f.readlines()
            data = []

            data_tmp = []
            for line in lines:
                if "---------------------" in line:
                    if len(data_tmp) > 0:
                        data_tmp.sort()
                        data.append(data_tmp[2:])
                        data_tmp = []
                    continue

                idx = int(line.split(':')[-1].strip())
                data_tmp.append(idx)
            
            # data写入文件
            print(data)
            for idx in range(max(50, len(data))):
                with open(file.replace('.txt', "_"+str(idx).zfill(3)+'_proc.txt'), 'w', encoding='utf-8') as f:
                    if idx+3 <= len(data):
                        for d in data[idx:idx+3]:
                            f.write(' '.join([str(i) for i in d]) + '\n')
                    else:
                        m = random.randint(0, len(data)-1)
                        m = min(m, len(data)-2)
                        m = max(m, 1)
                        l = random.randint(0, m-1)
                        r = random.randint(m+1, len(data)-1)
                        for rid in [l, m, r]:
                            f.write(' '.join([str(i) for i in data[rid]]) + '\n')
                        

