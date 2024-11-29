import argparse
import glob
import hashlib
import json
import logging
import os
import queue
import threading
from dataclasses import dataclass, field
from io import BytesIO
from pathlib import Path
from typing import Dict, List

import boto3
import botocore

from docling_parse import pdf_parser_v2

# Configure logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s - %(levelname)s - %(message)s"
)


@dataclass
class FileTask:
    folder_name: str

    file_name: str  # Local path where the file will be processed or saved
    file_hash: str


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Process S3 files using multithreading."
    )
    parser.add_argument(
        "-d", "--directory", help="input directory with pdf files", required=True
    )
    parser.add_argument(
        "-r",
        "--recursive",
        help="recursively finding pdf-files",
        required=False,
        default=False,
    )
    parser.add_argument(
        "-t",
        "--threads",
        required=False,
        help="processing threads",
        default=1,
    )

    # Restrict log-level to specific values
    parser.add_argument(
        "-l",
        "--log-level",
        type=str,
        choices=["info", "warning", "error", "fatal"],
        required=False,
        default="fatal",
        help="Log level [info, warning, error, fatal]",
    )

    args = parser.parse_args()

    return args.directory, args.recursive, int(args.threads), args.loglevel


def fetch_files_from_disk(directory, recursive, task_queue):
    """Recursively fetch files from disk and add them to the queue."""
    logging.info(f"Fetching file keys from disk: {directory}")

    for filename in sorted(glob.glob(os.path.join(directory, "*.pdf"))):

        file_name = str(Path(filename).resolve())

        hash_object = hashlib.sha256(filename.encode())
        file_hash = hash_object.hexdigest()

        # Create a FileTask object
        task = FileTask(folder_name=directory, file_name=file_name, file_hash=file_hash)
        task_queue.put(task)

    task_queue.put(None)
    logging.info("Done with queue")


def process_files_from_queue(file_queue, loglevel):
    """Process files from the queue."""

    while True:
        task = file_queue.get()

        if task is None:  # End of queue signal
            break

        logging.info(
            f"Queue-size [{file_queue.qsize()}], Processing task: {task.file_name}"
        )

        try:
            parser = pdf_parser_v2(loglevel)

            success = parser.load_document(task.file_hash, str(task.file_name))

            num_pages = parser.number_of_pages(task.file_hash)
            logging.info(f" => #-pages of {task.file_name}: {num_pages}")

            json_doc = parser.parse_pdf_from_key(task.file_hash)

            # with open(os.path.join(task.folder_name, f"{task.file_name}.json"), "w") as fw:
            with open(f"{task.file_name}.json", "w") as fw:
                fw.write(json.dumps(json_doc, indent=2))
        except:
            continue

        """
        try:
            if success:

                # Get number of pages
                num_pages = parser.number_of_pages(task.file_hash)
                logging.info(f" => #-pages of {task.file_name}: {num_pages}")
                
                # Parse page by page to minimize memory footprint
                for page in range(0, num_pages):
                    try:
                        json_doc = parser.parse_pdf_from_key_on_page(task.file_hash, page)

                        with open(os.path.join(directory, f"{task.file_hash}-page-{page:03}.json"), "w") as fw:
                            fw.write(json.dumps(json_doc, indent=2))
                        
                    except:
                        logging.error(f"problem with parsing {task.file_name} on page {page}")
            else:
                logging.error(f"problem with loading {task.file_name}")

            # Unload the (QPDF) document and buffers
            parser.unload_document(task.file_hash)

        except:
            logging.error(f"Error on file: {task}")
        """


def main():

    directory, recursive, threads, loglevel = parse_arguments()

    task_queue = queue.Queue()

    if threads == 1:

        fetch_files_from_disk(directory, recursive, task_queue)

        process_files_from_queue(task_queue, loglevel)

    """
    # Create threads
    fetch_thread = threading.Thread(
        target=fetch_files_from_s3,
        args=(
            directory,
            recursive
            file_queue,
        ),
    )
    process_threads = []

    for i in range(0, threads):
        process_threads.append(threading.Thread(
            target=process_files_from_queue, args=(file_queue)
        ))

    # Start threads
    fetch_thread.start()

    for _ in process_threads:
        _.start()

    # Wait for threads to complete
    fetch_thread.join()
    for _ in process_threads:
        _.join()
    """

    logging.info("All files processed successfully.")


if __name__ == "__main__":
    main()
