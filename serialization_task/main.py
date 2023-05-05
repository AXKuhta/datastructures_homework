from enum import Enum
import codecs
import json

# Функция, которая поможет json.dumps понять, что требуется сделать с неизвестными классами
def serialization_assist(x):
	if isinstance(x, Widget):
		widget_dict = vars(x)
		del widget_dict['parent']
		if widget_dict['childrens'] == []:
			del widget_dict['childrens']
		return { x.__class__.__name__: widget_dict }
	elif isinstance(x, Enum):
		return x.value
	else:
		return str(x)

class Alignment(Enum):
	HORIZONTAL = 1
	VERTICAL = 2

class Widget():

	def __init__(self, parent):
		self.parent = parent
		self.childrens = []
		if self.parent is not None:
			self.parent.add_children(self)

	def add_children(self, children: "Widget"):
		self.childrens.append(children)

	def to_binary(self):
		objects = json.dumps(self, default=serialization_assist, separators=(',', ':'))
		uncompressed = objects.encode()
		return codecs.encode(uncompressed, "zlib")

	@classmethod
	def from_binary(self, data):
		uncompressed = codecs.decode(data, "zlib")
		objects = json.loads(uncompressed)

		return self.from_dict(objects)


	@classmethod
	def from_dict(self, src_dict, parent=None):
		classlist = {
			"MainWindow": MainWindow,
			"Layout": Layout,
			"LineEdit": LineEdit,
			"ComboBox": ComboBox
		}

		for k, v in src_dict.items():
			cls = classlist[k]
			args = dict(v)

			if "childrens" in args:
				childrens = args["childrens"]
				del args["childrens"]
			else:
				childrens = []

			if parent is not None:
				args["parent"] = parent

			base = cls(**args)

			base.childrens = [Widget.from_dict(x, base) for x in childrens]

		return base

	def __str__(self):
		return f"{self.__class__.__name__}{self.childrens}"

	def __repr__(self):
		return str(self)

class MainWindow(Widget):

	def __init__(self, title: str):
		super().__init__(None)
		self.title = title

class Layout(Widget):

	def __init__(self, parent, alignment: Alignment):
		super().__init__(parent)
		self.alignment = alignment

class LineEdit(Widget):

	def __init__(self, parent, max_length: int=10):
		super().__init__(parent)
		self.max_length = max_length

class ComboBox(Widget):

	def __init__(self, parent, items):
		super().__init__(parent)
		self.items = items

app = MainWindow("Application")
layout1 = Layout(app, Alignment.HORIZONTAL)
layout2 = Layout(app, Alignment.VERTICAL)

edit1 = LineEdit(layout1, 20)
edit2 = LineEdit(layout1, 30)

box1 = ComboBox(layout2, [1, 2, 3, 4])
box2 = ComboBox(layout2, ["a", "b", "c"])

print(app)

bts = app.to_binary()
print(f"Binary data length {len(bts)}")

new_app = MainWindow.from_binary(bts)
print(new_app)
