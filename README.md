This project is designed to split a .mbox file into something a bit more manageable to read.

.mbox files contain email data, and are generated by Google Takeout. They contain the text, formatting, attachments and metadata for every email in one's email account.

mboxsplit.exe has 2 arguments. The first is the name of the .mbox file. The second is the name of the folder the "decoded" emails will be in. If the file does not exist, one will be created.

each email will be represented as a folder that contains a text file (1 per email) and one image file for each image in the email. The title of the folder contains the date and the name of the sender/receiver.

The text file inside each email contains the text of the email and some metadata including other recipients, subject, etc.

The image files currently contain jpgs that are encoded with base 64. still need to add something to decode the images.
