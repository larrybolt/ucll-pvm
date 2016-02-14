require 'MetaData'
require 'Html'
require 'Code'
require 'Cpp'
require 'Upload'
require 'Html'
require 'pathname'


module Quiz
  extend Html::Generation::Quiz
end


class Context
  include Contracts::TypeChecking
  include Html::Generation

  def initialize
    @last_exercise_index = 0
  end

  def increment_exercise_counter
    @last_exercise_index += 1
  end

  def compile(path)
    typecheck do
      assert(path: file)
    end

    Cpp.compile(path)
  end

  def format_source_file(path)
    typecheck do
      assert(path: file)
    end
    
    %{<div class="code"><pre>#{Code.format_file(path).strip}</pre></div>}
  end

  def format_inline(source, file: 'temp.cpp')
    typecheck do
      assert(source: string, file: string | pathname)
    end

    if String === file
    then path = Pathname.new file
    else path = file
    end

    typecheck do
      assert(file: pathname)
    end

    source = Code.remove_redundant_indentation(source)

    File.open(path, 'w') do |out|
      out.puts source
    end

    format_source_file(path)
  end

  def interpret_exercise(source, input: nil)
    typecheck do
      assert(source: string)
    end

    current_exercise_index = increment_exercise_counter

    basename = "temp#{current_exercise_index}"
    source_path = Pathname.new "#{basename}.cpp"
    executable_path = Pathname.new "#{basename}.exe"

    formatted_source = format_inline(source, file: source_path)
    
    Cpp.compile source_path

    if input
      output = `echo #{input} | #{executable_path}`.strip
      input_message = %{<p>Input: <code>#{input}</code></p>}
    else
      output = `#{executable_path}`.strip
      input_message = ""
    end

    format_exercise(<<-END, index: current_exercise_index)
      <p>What is the output of the following code?</p>
      #{formatted_source}
      #{input_message}
      <p>Output: #{Quiz.validated_input { verbatim output }}</p>
    END
  end

  def format_exercise(body, index: increment_exercise_counter)
    typecheck do
      assert(body: string)
    end

    <<-END
    <section class="question">
      <h1>Exercise #{index}</h1>
      #{body}
    </section>
    END
  end
end


meta_object do
  extend MetaData::Actions
  extend Html::Actions
  extend Upload::Mixin

  def remote_directory
    world.parent.remote_directory
  end

  html_template('assignment', context: Context.new, group_name: 'html')
  
  uploadable('assignment.html')
  upload_action

  group_action(:full, [:upload])
end
