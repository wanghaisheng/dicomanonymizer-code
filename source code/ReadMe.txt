=== Public Description: ===

Replaces the patient names in all the DICOM files in a folder (and sub-folders) with other strings as a batch. Works fast.

Anonymizing is required for medical image processing.


=== Registration Description: ===

When working on medical images (in DICOM format), the first thing to do is to remove the patient's identification from the DICOM image files. This is called anonymize. It is required for HIPAA compliance (http://www.hipaadvisory.com/REGS/HIPAAprimer.htm).

Although many commercial softwares on medical imaging do provide DICOM anonymizer, they are not easy to use because of:
* Most of them don't have batch function. The free trial version mostly limit your usage on a single file.
* They are slow. Modern CT scanners generates gigabytes of image data per scan, hence speed is very much appreciated.

Started by a programmer who needs to anonymize hundreds of patient data each time, this DICOM anonymizer project overcomes these limitations and make it easy and fast. Highlights are:

* Can handle many patients at a time as a batch.
* Replaces strings in DICOM files directly. It does not change the length of the DICOM tag. So it works very fast.
* It does not change Study Instance UID or Series Instance UID.
* Besides patient's name, the user can input any DICOM tag to replace.
* When a batch process is done, a list of original patient name and new name is for the user to copy.


=== History ===
Version	 Changes
1.1.6.1  Fixed bug: Corrected DICOM tag of "Study Description"
1.1.6.0  Fixed bug: Couldn't anonymize some dose report file.
         Fixed bug: Longer name could be typed in the list but will actually be trimmed.
		 Improvement: More detailed error message.
		 Improvement: Added a combo box for the DICOM tag's description
		 Improvement: Added a "Browse..." button.
