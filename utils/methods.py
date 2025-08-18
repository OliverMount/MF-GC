 # necessary methods can be imported from here

def print_status(msg,kind='short',line=None):
	
	if line is None:
		line=len(msg)

	if kind=='short':
		print('++ '+msg)
	else:
		print('++ '+line*'-')
		print('++ '+msg)
		print('++ '+line*'-') 


def print_shape(a,var_name=None):
	if var_name is None:
		var_name='given variable'

	print_status('Shape of '+ var_name + ' is '+ str(a.shape))
