#include "s3_storage.h"

#include <aws/core/Aws.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/s3/model/DeleteObjectRequest.h>

#include <iostream>

bool uploadStringToS3(const std::string& bucket_name,
                      const std::string& key,
                      const std::string& content) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucket_name.c_str());
    request.SetKey(key.c_str());

    auto input_data =
        Aws::MakeShared<Aws::StringStream>("UploadStringToS3");
    (*input_data) << content;

    request.SetBody(input_data);

    Aws::S3::Model::PutObjectOutcome outcome = s3_client.PutObject(request);

    if (!outcome.IsSuccess()) {
        std::cerr << "S3 PutObject failed: "
                  << outcome.GetError().GetMessage() << std::endl;
        return false;
    }

    return true;
}

bool downloadStringFromS3(const std::string& bucket_name,
                          const std::string& key,
                          std::string& output_content) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucket_name.c_str());
    request.SetKey(key.c_str());

    Aws::S3::Model::GetObjectOutcome outcome = s3_client.GetObject(request);

    if (!outcome.IsSuccess()) {
        std::cerr << "S3 GetObject failed: "
                  << outcome.GetError().GetMessage() << std::endl;
        return false;
    }

    std::ostringstream ss;
    ss << outcome.GetResult().GetBody().rdbuf();
    output_content = ss.str();

    return true;
}

bool listObjectsInS3(const std::string& bucket_name,
                     std::vector<std::string>& object_keys) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::ListObjectsV2Request request;
    request.SetBucket(bucket_name.c_str());

    Aws::S3::Model::ListObjectsV2Outcome outcome =
        s3_client.ListObjectsV2(request);

    if (!outcome.IsSuccess()) {
        std::cerr << "S3 ListObjectsV2 failed: "
                  << outcome.GetError().GetMessage() << std::endl;
        return false;
    }

    const auto& objects = outcome.GetResult().GetContents();
    for (const auto& object : objects) {
        object_keys.push_back(object.GetKey());
    }

    return true;
}

bool deleteObjectFromS3(const std::string& bucket_name,
                        const std::string& key) {
    Aws::S3::S3Client s3_client;

    Aws::S3::Model::DeleteObjectRequest request;
    request.SetBucket(bucket_name.c_str());
    request.SetKey(key.c_str());

    Aws::S3::Model::DeleteObjectOutcome outcome =
        s3_client.DeleteObject(request);

    if (!outcome.IsSuccess()) {
        std::cerr << "S3 DeleteObject failed: "
                  << outcome.GetError().GetMessage() << std::endl;
        return false;
    }

    return true;
}