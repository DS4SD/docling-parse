import argparse
import hashlib
import json
import logging
import os
import queue
import threading
from typing import Dict, List

import boto3
import botocore

from docling_parse import pdf_parser_v2

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Process S3 files using multithreading."
    )
    parser.add_argument(
        "-u",
        "--endpoint-url",
        help="S3 url",
        default="https://s3.eu-de.cloud-object-storage.appdomain.cloud/",
    )
    parser.add_argument("-a", "--access-key", required=True, help="Access Key")
    parser.add_argument("-s", "--secret-key", required=True, help="Secret Key")
    parser.add_argument(
        "-r",
        "--region",
        help="region (eg: us-east-1,eu-de-standard)",
        required=False,
        default="eu-de-standard",
    )
    parser.add_argument(
        "-b",
        "--bucket-name",
        required=False,
        help="S3 Bucket name",
        default="doclaynet",
    )
    parser.add_argument(
        "-p",
        "--bucket-prefix",
        required=False,
        help="S3 Bucket prefix",
        default="source_docs/Extra/",
    )
    parser.add_argument(
        "-l",
        "--local-scratch",
        required=False,
        help="local scratch directory",
        default="./scratch",
    )

    return parser.parse_args()


def list_buckets(s3_client):

    # List the buckets
    response = s3_client.list_buckets()

    # Print the bucket names
    logging.info("------------------------\nBuckets:")
    for bucket in response.get("Buckets", []):
        logging.info(f"- {bucket['Name']}")


def fetch_files_from_s3(
    local_dir, s3_client, bucket_name, top_level_prefix, prefix, file_queue
):
    """Recursively fetch file keys from the S3 bucket and add them to the queue."""
    logging.info(f"Fetching file keys from S3 bucket with prefix: {prefix}")
    # local_files = glob.glob(os.path.join(args.local_scratch, "*.pdf"))

    paginator = s3_client.get_paginator("list_objects_v2")
    for page in paginator.paginate(Bucket=bucket_name, Prefix=prefix, Delimiter="/"):
        # Handle files in the current prefix
        for obj in page.get("Contents", []):

            filename = os.path.basename(obj["Key"])
            if filename.endswith(".pdf"):

                # Download and process file (placeholder for actual processing logic)
                response = s3_client.get_object(Bucket=bucket_name, Key=obj["Key"])
                data = response["Body"].read()

                # Generate a hash (e.g., SHA-256)
                hash_object = hashlib.sha256(data)
                file_hash = hash_object.hexdigest()

                local_file = os.path.join(local_dir, f"{file_hash}.pdf")

                # Save the data
                logging.info(f"processing file: {filename} -> {local_file}")
                with open(local_file, "wb") as fw:
                    fw.write(data)

                # file_queue.put(obj["Key"])
                file_queue.put((filename, file_hash, local_file))

        # Recursively handle sub-prefixes (folders)
        for common_prefix in page.get("CommonPrefixes", []):
            sub_prefix = common_prefix["Prefix"]
            logging.info(f"Entering sub-prefix: {sub_prefix}")
            fetch_files_from_s3(
                local_dir,
                s3_client,
                bucket_name,
                top_level_prefix,
                sub_prefix,
                file_queue,
            )

    # Signal the end of fetching (only add `None` at the top level)
    if top_level_prefix == prefix:  # Only at the top-level call
        file_queue.put(None)


def print_toc(toc):

    if isinstance(toc, List):
        for _ in toc:
            print_toc(_)

    elif isinstance(toc, Dict):
        if "title" in toc and "level" in toc:
            print(" " * (4 * toc["level"]), toc["title"])

        if "children" in toc:
            for _ in toc["children"]:
                print_toc(_)


def process_files_from_queue(s3_client, bucket_name, file_queue):

    parser = pdf_parser_v2("error")

    """Process files from the queue."""
    while True:
        item = file_queue.get()
        if item is None:  # End of queue signal
            break

        try:
            logging.info(f"Processing file: {item}")
            success = parser.load_document(item[1], item[2])

            if success:

                # Get number of pages
                num_pages = parser.number_of_pages(item[1])
                # logging.info(f" => #-pages: {num_pages}")

                out = parser.get_annotations(item[1])
                logging.info(f"{item[1]} [{num_pages}]: {out.keys()}")
                for key, val in out.items():
                    if val is not None and key == "table_of_contents":
                        print_toc(val)

                    elif val is not None:
                        logging.info(f" => {key}: 1")

                """
                # Parse page by page to minimize memory footprint
                for page in range(0, num_pages):
                    json_doc = parser.parse_pdf_from_key_on_page(item[1], page)
                """

                logging.info(f"Processed {item[1]}")
            else:
                logging.error(f"problem with {item[1]}")

            # Unload the (QPDF) document and buffers
            parser.unload_document(item[1])

            os.remove(item[2])
            logging.info(f"deleted file: {item[2]}")

        except:
            logging.error(f"Error on file: {item}")


def main():

    args = parse_arguments()

    os.makedirs(args.local_scratch, exist_ok=True)

    session = boto3.session.Session()

    config = botocore.config.Config(connect_timeout=60, signature_version="s3v4")

    # Initialize the S3 client
    s3_client = session.client(
        "s3",
        endpoint_url=args.endpoint_url,
        verify=True,
        aws_access_key_id=args.access_key,
        aws_secret_access_key=args.secret_key,
        region_name=args.region,
        config=config,
    )

    file_queue = queue.Queue()

    # Create threads
    fetch_thread = threading.Thread(
        target=fetch_files_from_s3,
        args=(
            args.local_scratch,
            s3_client,
            args.bucket_name,
            args.bucket_prefix,
            args.bucket_prefix,
            file_queue,
        ),
    )
    process_thread = threading.Thread(
        target=process_files_from_queue, args=(s3_client, args.bucket_name, file_queue)
    )

    # Start threads
    fetch_thread.start()
    process_thread.start()

    # Wait for threads to complete
    fetch_thread.join()
    process_thread.join()

    logging.info("All files processed successfully.")


if __name__ == "__main__":
    main()
