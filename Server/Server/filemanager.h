#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>


template <typename T>
class FileManager {
public:
    FileManager(const std::string &filename);

    FileManager(const FileManager &manager) = delete;
    FileManager& operator=(const FileManager &manager) = delete;

    FileManager(FileManager &&manager) = default;
    FileManager& operator=(FileManager &&manager) = default;

    ~FileManager();

    void Push(T value);
    T Pop();
    bool Empty() const;
private:
    bool is_empty_;
    std::fstream stream_;
    std::string filename_;
    std::mutex queue_mutex_;
    std::condition_variable queue_variable_;
};


template<typename T>
FileManager<T>::FileManager(const std::string &filename) :
    filename_(filename), is_empty_(true)
{
}

template<typename T>
FileManager<T>::~FileManager()
{
    stream_.close();
}

template<typename T>
void FileManager<T>::Push(T value) {
    std::unique_lock<std::mutex> locker(queue_mutex_);
    queue_variable_.wait(locker, [this]{return is_empty_;});
    stream_.open(filename_, std::fstream::out);
    stream_ << value;
    is_empty_ = false;
    stream_.close();
    queue_variable_.notify_one();
}


template<typename T>
T FileManager<T>::Pop() {
    std::unique_lock<std::mutex> locker(queue_mutex_);
    queue_variable_.wait(locker, [this]{return !is_empty_;});
    T value;
    stream_.open(filename_, std::fstream::in);
    stream_ >> value;
    stream_.close();
    stream_.open(filename_, std::fstream::out);
    stream_.close();
    is_empty_ = true;
    return value;
}

template<typename T>
bool FileManager<T>::Empty() const {
    return is_empty_;
}

#endif // FILEMANAGER_H
