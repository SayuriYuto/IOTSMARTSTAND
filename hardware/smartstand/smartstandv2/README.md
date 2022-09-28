to write the fetch query use : 
https://cloud.google.com/firestore/docs/reference/rest/v1/StructuredQuery

"select task from task"
task is field
task is collectionid

query.set("select/fields/fieldPath", "task");
query.set("from/collectionId", "task");
query.set("from/allDescendants", true);
query.set("limit", 1);

using firebaseJson library to parse json
very complicated

got into json 

finally fucking done