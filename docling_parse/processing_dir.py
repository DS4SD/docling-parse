import argparse
import hashlib
import json
import logging
import os
import queue
import threading
from typing import Dict, List
from dataclasses import dataclass, field

from io import BytesIO

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
        "-d",
        "--directory",
        help="input directory with pdf files",
        required=True
    )
    parser.add_argument(
        "-r",
        "--recursive",
        help="recursively finding pdf-files",
        required=False,
        default=False
    )
    parser.add_argument(
        "-t",
        "--threads",
        required=False,
        help="processing threads",
        default=4,
    )

    args = parser.parse_args()

    return args.directory, args.recursive, int(args.threads)


def fetch_files_from_disk(directory, recursive, task_queue):
    """Recursively fetch files from disk and add them to the queue."""
    logging.info(f"Fetching file keys from disk: {directory}")
    
    for filename in glob.glob(os.path.join(directory, "*.pdf")):

        # Create a FileTask object
        task = FileTask(folder_name=directory, file_name=Path(filename).resolve(), file_hash="")
        task_queue.put(task)

    task_queue.put(None)
    logging.info("Done with queue")

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


def process_files_from_queue(file_queue):
    """Process files from the queue."""
    
    parser = pdf_parser_v2("fatal")

    while True:
        task = file_queue.get()

        if task is None:  # End of queue signal
            break

        task = retrieve_file(s3_client, task)
        logging.info(f"Queue-size [{file_queue.qsize()}], Processing task: {task.file_name}")
        
        try:
            success = parser.load_document_from_bytesio(task.file_hash, task.data)
            
            if success:

                # Get number of pages
                num_pages = parser.number_of_pages(task.file_hash)
                logging.info(f" => #-pages of {task.file_name}: {num_pages}")

                """
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
                    try:
                        json_doc = parser.parse_pdf_from_key_on_page(task.file_hash, page)
                    except:
                        save_file(scratch_dir, task)
                        logging.error(f"problem with parsing {task.file_name} on page {page}")
            else:
                logging.error(f"problem with loading {task.file_name}")

            # Unload the (QPDF) document and buffers
            parser.unload_document(task.file_hash)

        except:
            logging.error(f"Error on file: {task}")

def main():

    directory, recursive, threads = parse_arguments()
    
    file_queue = queue.Queue()

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

    logging.info("All files processed successfully.")


if __name__ == "__main__":
    main()
