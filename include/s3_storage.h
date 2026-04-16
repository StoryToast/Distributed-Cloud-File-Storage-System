#ifndef S3_STORAGE_H
#define S3_STORAGE_H

#include <string>
#include <vector>

bool uploadStringToS3(const std::string& bucket_name,
                      const std::string& key,
                      const std::string& content);

bool downloadStringFromS3(const std::string& bucket_name,
                          const std::string& key,
                          std::string& output_content);

bool listObjectsInS3(const std::string& bucket_name,
                     std::vector<std::string>& object_keys);

bool deleteObjectFromS3(const std::string& bucket_name,
                        const std::string& key);

#endif