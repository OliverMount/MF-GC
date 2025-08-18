# Loading database and test a few queires

import os
import sqlite3

os.chdir('/media/oli/Research/Gitrepo/GranularLayerModel/connectivity/')

conn = sqlite3.connect('mf_grc.db')   # This will create a database or access the one if it does exists
cursor = conn.cursor()   # cursor is a iterator used for executing commands or accessing data


## Display from node table
cursor.execute('SELECT * FROM worker')  # Just 5 entries
rows = cursor.fetchall()
for row in rows:
    print(row)


## Display from node table
cursor.execute('SELECT * FROM cell LIMIT 5')  # Just 5 entries
rows = cursor.fetchall()
for row in rows:
    print(row)
    
    
    
## Display from node table
cursor.execute('SELECT * FROM cell where cell_type = "mf" ')  # Just 5 entries
rows = cursor.fetchall()
#for row in rows:
#    print(row)
    
print("No. of mossy fiber is ", len(rows))
print("Minimum gid number of the mf is  ", min([row[0] for row in rows]))
print("Maximum gid number of the mf is  ", max([row[0] for row in rows]))
    
cursor.execute('SELECT * FROM cell where cell_type = "grc" ')  # Just 5 entries
rows = cursor.fetchall()
#for row in rows:
#    print(row)
    
print("No. of grc is ", len(rows))
print("Minimum gid number of the grc is  ", min([row[0] for row in rows]))
print("Maximum gid number of the grc is  ", max([row[0] for row in rows]))

# Display from edges table
cursor.execute('SELECT * FROM synapse LIMIT 5')  # Just 5 entries
rows = cursor.fetchall()
for row in rows:
    print(row)
    
     
## Get the targets where the source is '0' 
cursor.execute('''
    SELECT source_gid,target_gid FROM connectivity
    WHERE source_gid = 0
''')
print(cursor.fetchall())


## Get the source where the target is '0' 
cursor.execute('''
    SELECT source_gid,target_gid FROM connectivity
    WHERE target_gid = 2988
''')
print(cursor.fetchall())


## To get the list of tables

cursor.execute("""
    SELECT name FROM sqlite_master 
    WHERE type='table';
""")

# Fetch and print the list of tables
tables = cursor.fetchall()
print("List of tables in the database:")
for table in tables:
    print(table[0])


# Workder nodes 
cursor.execute(''' SELECT * FROM worker ''')
print(cursor.fetchall())  

cursor.execute('''
    SELECT * FROM nodes 
''')
print(cursor.fetchall()) 

 
#  Check if edge exists between two nodes
cursor.execute('''
    SELECT EXISTS(
        SELECT 1 FROM edges
        WHERE source = ? AND target = ?
    )
''', ('mf_138__67', 'grc_1977'))
print(cursor.fetchone()[0])  # Returns 1 (True) or 0 (False)


#Validate Edge Existence 
cursor.execute('''
    SELECT EXISTS(
        SELECT 1 FROM edges
        WHERE source = ? AND target = ?
    )
''', ('mf_138__67', 'grc_1977'))
print(cursor.fetchone()[0]) 
# Returns 1 (exists) or 0 (doesn't exist) 



